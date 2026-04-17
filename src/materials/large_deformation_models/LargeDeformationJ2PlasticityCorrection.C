//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADRankTwoTensorForward.h"
#include "EigenADReal.h"
#include "LargeDeformationJ2PlasticityCorrection.h"
#include "LargeDeformationPlasticityModel.h"
#include "MooseError.h"
#include "MooseTypes.h"
#include "RaccoonUtils.h"
#include "RankTwoTensorForward.h"
#include <cmath>
#include "Qp_Mapping.h"

registerMooseObject("raccoonApp", LargeDeformationJ2PlasticityCorrection);

InputParameters
LargeDeformationJ2PlasticityCorrection::validParams()
{
  InputParameters params = LargeDeformationPlasticityModel::validParams();

  params.addClassDescription("Large deformation $J_2$ plasticity. The exponential constitutive "
                             "update is used to update the plastic deformation.");
  params.addRequiredParam<MaterialPropertyName>("K", "K");
  params.addRequiredParam<MaterialPropertyName>("G", "G");
  params.addRequiredCoupledVar("phase_field", "Name of the phase-field (damage) variable");
  params.addParam<MaterialPropertyName>(
      "elastic_degradation_function", "g", "The elastic degradation function");
  params.addParam<MaterialPropertyName>(
      "strain_energy_density_corr",
      "psie_corr",
      "Name of the strain energy density computed by this material model");
  params.addParam<Real>("num_qps", 8, "Number of QPs");
  params.addParam<Real>("d1", 0.1, "d1 triax");
  params.addParam<Real>("d2", 3.8, "d2 triax");
  params.addParam<Real>("d3", -1.8, "d3 triax");
  params.addParam<Real>("triax_gaussian_peak",
                        2.0,
                        "Peak value of the Gaussian triaxiality function f(η) at η=0. "
                        "f(η) = 1 + (peak-1)*exp(-η²/(2σ²)), so the asymptotic value "
                        "at |η|→∞ is always 1 and f(0)=peak. Default 2 recovers the "
                        "original fixed-amplitude behavior.");
  params.addRequiredParam<MooseEnum>(
      "element", MooseEnum(QpMapping::ELEMENT_ENUM_DEFINITION), "The element type");
  params.addParam<bool>("apply_strain_energy_split",
                        true,
                        "Whether to apply volumetric/deviatoric split to the active strain "
                        "energy (psie_active). If true (default), only the positive part of the "
                        "energy is assigned to psie_active. If false, the full unsplit energy U + "
                        "W is used for psie_active.");
  params.addParam<MaterialPropertyName>(
      "hardening_plastic_energy_density_active",
      "psip_active",
      "Name of the active plastic energy density declared by the hardening model "
      "(used for psip_triax). Must match the hardening model's declared property name.");
  params.addParam<Real>("psip_triax_threshold",
                        200.0,
                        "Activation threshold for psip_triax. psip_active/triaxfunc must exceed "
                        "this value before psip_triax becomes non-zero.");
  params.addParam<bool>("recover_psip_triax",
                        false,
                        "If true (and recover=true), read psip_triax from the restart solution.");
  return params;
}

LargeDeformationJ2PlasticityCorrection::LargeDeformationJ2PlasticityCorrection(
    const InputParameters & parameters)
  : LargeDeformationPlasticityModel(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _bebar(declareADProperty<RankTwoTensor>(prependBaseName("be_bar"))),
    _bebar_old(getMaterialPropertyOldByName<RankTwoTensor>(prependBaseName("be_bar"))),
    _bebar_det(declareADProperty<Real>(prependBaseName("be_bar_det"))),
    // _f(declareADProperty<RankTwoTensor>(prependBaseName("incremental_deformation_gradient"))),
    _G(getADMaterialPropertyByName<Real>("G")),
    _K(getADMaterialPropertyByName<Real>("K")),
    _F_old(getMaterialPropertyOldByName<RankTwoTensor>(prependBaseName("deformation_gradient"))),
    _F(getADMaterialPropertyByName<RankTwoTensor>(prependBaseName("deformation_gradient"))),
    _d_name(getVar("phase_field", 0)->name()),
    // The degradation function and its derivatives
    _ge_name(prependBaseName("elastic_degradation_function", true)),
    _ge(getADMaterialProperty<Real>(_ge_name)),
    _dge_dd(getADMaterialProperty<Real>(derivativePropertyName(_ge_name, {_d_name}))),

    // The strain energy density and its derivatives
    _psie_name(prependBaseName("strain_energy_density_corr", true)),
    _psie_corr(declareADProperty<Real>(_psie_name)),
    _psie_active_corr(declareADProperty<Real>(_psie_name + "_active")),
    _dpsie_dd_corr(declareADProperty<Real>(derivativePropertyName(_psie_name, {_d_name}))),
    _psie_unsplit(declareADProperty<Real>(_psie_name + "_unsplit")),
    _triaxfunc(declareADProperty<Real>(prependBaseName("triaxfunc"))),
    _triaxiality_kirchhoff(declareADProperty<Real>(prependBaseName("triaxiality_kirchhoff"))),
    _triaxiality_cauchy(declareADProperty<Real>(prependBaseName("triaxiality_cauchy"))),
    _d1(getParam<Real>("d1")),
    _d2(getParam<Real>("d2")),
    _d3(getParam<Real>("d3")),
    _triax_gaussian_peak(getParam<Real>("triax_gaussian_peak")),
    _element(getParam<MooseEnum>("element").getEnum<QpMapping::Element>()),
    _apply_strain_energy_split(getParam<bool>("apply_strain_energy_split")),
    _psip_active_ref(getADMaterialProperty<Real>(
        getParam<MaterialPropertyName>("hardening_plastic_energy_density_active"))),
    _psip_triax_threshold(getParam<Real>("psip_triax_threshold")),
    _recover_psip_triax(getParam<bool>("recover_psip_triax")),
    _psip_triax_raw(declareADProperty<Real>(prependBaseName("psip_triax_raw"))),
    _psip_triax_raw_old(getMaterialPropertyOld<Real>(prependBaseName("psip_triax_raw"))),
    _psip_triax(declareADProperty<Real>(prependBaseName("psip_triax"))),
    _psip_triax_old(getMaterialPropertyOld<Real>(prependBaseName("psip_triax")))
{
  _check_range = true;
  if (_recover)
    _lookup = QpMapping::getLookup(_element, _qpnum, /*reversed=*/true);
}

void
LargeDeformationJ2PlasticityCorrection::initQpStatefulProperties()
{
  _Fp[_qp].setToIdentity();
  _ep[_qp] = 0;
  _bebar[_qp].setToIdentity();
  _psip_triax_raw[_qp] = 0.0;
  _psip_triax[_qp] = 0.0;
  unsigned int qp_max = _qpnum;

  auto formatQP = [qp_max](unsigned int qp)
  {
    if (qp_max < 10)
      return std::to_string(qp); // Single digit
    else
      return (qp < 10) ? "0" + std::to_string(qp) : std::to_string(qp); // Two digits
  };

  std::vector<std::string> indices = {"x", "y", "z"};
  if (_recover)
  {
    unsigned int qp_sel = QpMapping::getQP(_qp + 1, _lookup);

    _ep[_qp] = _solution_object_ptr->pointValue(_t,
                                                _current_elem->true_centroid(),
                                                "effective_plastic_strain_" + formatQP(qp_sel),
                                                nullptr);
    if (_ep[_qp] < 0)
      _ep[_qp] = 0;
    for (int i_ind = 0; i_ind < 3; i_ind++)
      for (int j_ind = 0; j_ind < 3; j_ind++)
      {
        _bebar[_qp](i_ind, j_ind) = _solution_object_ptr->pointValue(
            _t,
            _current_elem->true_centroid(),
            "be_bar_" + indices[i_ind] + indices[j_ind] + "_" + formatQP(qp_sel),
            nullptr);
      }

    if (_recover_psip_triax)
    {
      _psip_triax_raw[_qp] = _solution_object_ptr->pointValue(
          _t, _current_elem->true_centroid(), "psip_triax_raw_" + formatQP(qp_sel), nullptr);
      if (MetaPhysicL::raw_value(_psip_triax_raw[_qp]) < 0)
        _psip_triax_raw[_qp] = 0.0;
    }

    // Re-enforce det(be_bar) = 1 on the mapped be_bar.
    // Scaling by 1/cbrt(det) restores the isochoric constraint while preserving
    // the trace and eigenvalue ratios (deviatoric shape). The deviatoric approach
    // via computeCorrectionTerm sets tr = 3*Ie_bar, which does not preserve the
    // physical trace and can inflate psie when the error has a volumetric component.
    const Real det_bebar = MetaPhysicL::raw_value(_bebar[_qp].det());
    if (std::abs(det_bebar - 1.0) > 1e-10)
      _bebar[_qp] /= std::cbrt(det_bebar);
  }
};

void
LargeDeformationJ2PlasticityCorrection::updateState(ADRankTwoTensor & stress,
                                                    ADRankTwoTensor & /*Fe*/)
{
  using std::cbrt;
  using std::exp;
  using std::log;
  using std::sqrt;
  ADRankTwoTensor I2;
  I2.setToIdentity();
  if (_recover && MetaPhysicL::raw_value(_ep[_qp]) < 0)
  {
    _ep[_qp] = 0;
  }
  // Assume no plastic increment
  ADReal delta_ep = 0;

  // Obtain incremental f
  ADRankTwoTensor f = _F[_qp] * _F_old[_qp].inverse();

  // Compute fbar
  ADRankTwoTensor fbar = f / cbrt(f.det());

  // Compute bebar_trial
  _bebar[_qp] = fbar * _bebar_old[_qp] * fbar.transpose();

  // Compute trial stress
  // Will need to do a better implementation of this
  ADRankTwoTensor s_trial = _G[_qp] * _ge[_qp] * _bebar[_qp].deviatoric();
  ADReal s_trial_norm = s_trial.doubleContraction(s_trial);
  if (MooseUtils::absoluteFuzzyEqual(s_trial_norm, 0))
    s_trial_norm.value() = libMesh::TOLERANCE * libMesh::TOLERANCE;

  // Norm of trial stress
  s_trial_norm = sqrt(s_trial_norm);

  // Check for plastic loading
  _Np[_qp] = sqrt(1.5) * s_trial / s_trial_norm;

  // Return mapping
  ADReal phi = computeResidual(s_trial_norm, delta_ep);
  if (phi > 0)
  {
    returnMappingSolve(s_trial_norm, delta_ep, _console);

    // Update stress
    ADRankTwoTensor s = s_trial - _ge[_qp] * _G[_qp] * delta_ep * _bebar[_qp].trace() * _Np[_qp];
    _ep[_qp] = _ep_old[_qp] + delta_ep;

    // Updating Kirchoff stress
    ADReal J = _F[_qp].det();
    ADReal p = 0.5 * _K[_qp] * (J * J - 1);
    ADRankTwoTensor tau = J >= 1.0 ? _ge[_qp] * p * I2 + s : p * I2 + s;
    stress = tau / J;
    ADRankTwoTensor devbebar = s / _ge[_qp] / _G[_qp];
    computeCorrectionTerm(devbebar);
  }
  else
  {
    // Updating Kirchoff stress
    ADReal J = _F[_qp].det();
    ADReal p = 0.5 * _K[_qp] * (J * J - 1);
    ADRankTwoTensor tau = J >= 1.0 ? _ge[_qp] * p * I2 + s_trial : p * I2 + s_trial;
    stress = tau / J;
    _ep[_qp] = _ep_old[_qp];
  }
  computeStrainEnergyDensity();
  _hardening_model->plasticEnergy(_ep[_qp]);
  _hardening_model->plasticDissipation(delta_ep, _ep[_qp], 0);

  // Compute psip_triax_raw: unthresholded triaxiality-weighted plastic energy with irreversibility
  // This is what gets recovered and tracks the full history without threshold.
  {
    const Real psi_a = MetaPhysicL::raw_value(_psip_active_ref[_qp]);
    const Real tf = MetaPhysicL::raw_value(_triaxfunc[_qp]);
    const Real old_raw = _psip_triax_raw_old[_qp];

    // Compute the new unthresholded value
    Real new_value = 0.0;
    if (psi_a >= 0.1 && tf > 0.0)
      new_value = psi_a / tf;

    // Apply irreversibility: value can only increase, never decrease
    if (new_value >= old_raw && new_value > 0.0)
      _psip_triax_raw[_qp] = _psip_active_ref[_qp] / _triaxfunc[_qp]; // new > old, use AD
    else
      _psip_triax_raw[_qp] = ADReal(old_raw); // old >= new, keep old (includes 0)
  }

  // Compute psip_triax: thresholded version derived from psip_triax_raw
  {
    const Real raw_value = MetaPhysicL::raw_value(_psip_triax_raw[_qp]);
    if (raw_value >= _psip_triax_threshold)
      _psip_triax[_qp] = _psip_triax_raw[_qp]; // above threshold, use full AD
    else
      _psip_triax[_qp] = ADReal(0.0); // below threshold, zero
  }

  // Avoiding NaN issues for rate depedent models
  if (_t_step > 0)
  {
    _heat[_qp] = _hardening_model->plasticDissipation(delta_ep, _ep[_qp], 1) * delta_ep / _dt;

    _heat[_qp] += _hardening_model->thermalConjugate(_ep[_qp]) * delta_ep / _dt;
  }
  else
    _heat[_qp] = 0;

  // Calculate triaxiality for both Kirchhoff and Cauchy stresses
  // Kirchhoff stress: tau = J * sigma
  ADReal J = _F[_qp].det();
  ADRankTwoTensor tau = J * stress;

  // Triaxiality for Kirchhoff stress: η_tau = τ_m / τ_eq
  // Mean Kirchhoff stress: τ_m = trace(τ) / 3
  ADReal tau_mean = tau.trace() / 3.0;
  // Von Mises equivalent Kirchhoff stress: τ_eq = sqrt(3/2 * s_tau : s_tau)
  ADRankTwoTensor s_tau = tau.deviatoric();
  ADReal s_tau_norm_sq = s_tau.doubleContraction(s_tau);
  if (MooseUtils::absoluteFuzzyEqual(s_tau_norm_sq, 0))
    s_tau_norm_sq.value() = libMesh::TOLERANCE * libMesh::TOLERANCE;
  ADReal tau_eq = sqrt(1.5 * s_tau_norm_sq);
  _triaxiality_kirchhoff[_qp] = tau_mean / tau_eq;

  // Triaxiality for Cauchy stress: η_sigma = σ_m / σ_eq
  // Mean Cauchy stress: σ_m = trace(σ) / 3
  ADReal sigma_mean = stress.trace() / 3.0;
  // Von Mises equivalent Cauchy stress: σ_eq = sqrt(3/2 * s_sigma : s_sigma)
  ADRankTwoTensor s_sigma = stress.deviatoric();
  ADReal s_sigma_norm_sq = s_sigma.doubleContraction(s_sigma);
  if (MooseUtils::absoluteFuzzyEqual(s_sigma_norm_sq, 0))
    s_sigma_norm_sq.value() = libMesh::TOLERANCE * libMesh::TOLERANCE;
  ADReal sigma_eq = sqrt(1.5 * s_sigma_norm_sq);
  _triaxiality_cauchy[_qp] = sigma_mean / sigma_eq;

  // Old Johnson-Cook damage function using Cauchy stress triaxiality: f(η) = d1 + d2 * exp(d3 * η)
  // _triaxfunc[_qp] = _d1 + _d2 * std::exp(MetaPhysicL::raw_value(_d3 * _triaxiality_cauchy[_qp]));

  // Gaussian damage function: f(η) = 1 + exp(-η²/2σ²)
  // Peak at η=0: f(0) = 2, minimum asymptotic value: f(±∞) = 1
  // Reaches ~1.1 at η = ±1
  ADReal eta = _triaxiality_cauchy[_qp];
  const Real sigma_sq = 0.1803; // σ² = 0.1803, so σ ≈ 0.4247
  _triaxfunc[_qp] = 1.0 + (_triax_gaussian_peak - 1.0) *
                               std::exp(MetaPhysicL::raw_value(-eta * eta / (2.0 * sigma_sq)));

  // Store determinant of bebar
  _bebar_det[_qp] = _bebar[_qp].det();

  // if (_current_elem->id() == 1)
  // {
  //   std::cout << "=================" << std::endl;
  //   MetaPhysicL::raw_value(_bebar_old[_qp]).print();

  //   MetaPhysicL::raw_value(_bebar[_qp]).print();
  //   std::cout << MetaPhysicL::raw_value(delta_ep) << std::endl;
  //   std::cout << "=================" << std::endl;
  // }
}

Real
LargeDeformationJ2PlasticityCorrection::computeReferenceResidual(
    const ADReal & effective_trial_stress, const ADReal & delta_ep)
{
  return MetaPhysicL::raw_value(effective_trial_stress - std::sqrt(3.0 / 2.0) * _ge[_qp] * _G[_qp] *
                                                             delta_ep * _bebar[_qp].trace());
}

ADReal
LargeDeformationJ2PlasticityCorrection::computeResidual(const ADReal & effective_trial_stress,
                                                        const ADReal & delta_ep)
{
  ADReal ep = _ep_old[_qp] + delta_ep;

  return effective_trial_stress -
         std::sqrt(2.0 / 3.0) * (_hardening_model->plasticEnergy(ep, 1) +
                                 _hardening_model->plasticDissipation(delta_ep, ep, 1)) -
         std::sqrt(3.0 / 2.0) * _ge[_qp] * _G[_qp] * delta_ep * _bebar[_qp].trace();
}

ADReal
LargeDeformationJ2PlasticityCorrection::computeDerivative(const ADReal & /*effective_trial_stress*/
                                                          ,
                                                          const ADReal & delta_ep)
{
  ADReal ep = _ep_old[_qp] + delta_ep;
  return -std::sqrt(2.0 / 3.0) * (_hardening_model->plasticEnergy(ep, 2) +
                                  _hardening_model->plasticDissipation(delta_ep, ep, 2)) -
         std::sqrt(3.0 / 2.0) * _ge[_qp] * _G[_qp] * _bebar[_qp].trace();
}

void
LargeDeformationJ2PlasticityCorrection::computeCorrectionTerm(const ADRankTwoTensor & devbebar)
{
  using std::cbrt;
  using std::sqrt;

  // Identity tensor
  ADRankTwoTensor I2(RankTwoTensorTempl<ADReal>::initIdentity);

  ADReal a = devbebar(0, 0);
  ADReal b = devbebar(1, 1);
  ADReal c = devbebar(2, 2);
  ADReal d = devbebar(1, 2);
  ADReal e = devbebar(0, 2);
  ADReal h = devbebar(0, 1);

  ADReal A = a + b + c;
  ADReal B = a * b + a * c + b * c - d * d - e * e - h * h;
  ADReal C = a * b * c + 2.0 * d * e * h - a * d * d - b * e * e - c * h * h - 1.0;

  ADReal D = cbrt(
      -2 * A * A * A +
      3 * sqrt(3) *
          sqrt(4 * A * A * A * C - A * A * B * B - 18 * A * B * C + 4 * B * B * B + 27 * C * C) +
      9 * A * B - 27 * C);

  ADReal Ie_bar = D / 3 / cbrt(2) - cbrt(2) * (3 * B - A * A) / 3 / D - A / 3;
  _bebar[_qp] = devbebar + Ie_bar * I2;
}

void
LargeDeformationJ2PlasticityCorrection::computeStrainEnergyDensity()
{
  using std::log;
  ADReal J = _F[_qp].det();
  ADReal U = 0.5 * _K[_qp] * (0.5 * (J * J - 1) - log(J));
  ADReal W = 0.5 * _G[_qp] * (_bebar[_qp].trace() - 3.0);

  // Compute unsplit total elastic energy
  _psie_unsplit[_qp] = U + W;

  // Compute split energies
  ADReal E_el_pos = J >= 1.0 ? U + W : W;
  ADReal E_el_neg = J >= 1.0 ? 0.0 : U;

  // Set psie_active based on whether split is applied
  _psie_active_corr[_qp] = _apply_strain_energy_split ? E_el_pos : _psie_unsplit[_qp];

  // Degraded energy (always uses split for stability)
  _psie_corr[_qp] = _ge[_qp] * E_el_pos + E_el_neg;

  _dpsie_dd_corr[_qp] = _dge_dd[_qp] * _psie_active_corr[_qp];
}
