//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "EigenADReal.h"
#include "LargeDeformationJ2PlasticityCorrection.h"
#include "CNHIsotropicElasticity.h"
#include "RaccoonUtils.h"

registerMooseObject("raccoonApp", LargeDeformationJ2PlasticityCorrection);

InputParameters
LargeDeformationJ2PlasticityCorrection::validParams()
{
  InputParameters params = LargeDeformationJ2PlasticityBase::validParams();
  params.addClassDescription("Large deformation $J_2$ plasticity using the bebar (modified "
                             "left Cauchy-Green) update. Elastic parameters are sourced from "
                             "the associated CNHIsotropicElasticity model.");
  params.addRequiredCoupledVar("phase_field", "Name of the phase-field (damage) variable");
  params.addParam<MaterialPropertyName>(
      "strain_energy_density_corr",
      "psie_corr",
      "Name of the strain energy density computed by this material model");
  params.addParam<bool>("apply_strain_energy_split",
                        true,
                        "Whether to apply volumetric/deviatoric split to the active strain "
                        "energy (psie_active). If true (default), only the positive part of the "
                        "energy is assigned to psie_active. If false, the full unsplit energy U + "
                        "W is used for psie_active.");
  return params;
}

LargeDeformationJ2PlasticityCorrection::LargeDeformationJ2PlasticityCorrection(
    const InputParameters & parameters)
  : LargeDeformationJ2PlasticityBase(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _bebar(declareADProperty<RankTwoTensor>(prependBaseName("be_bar"))),
    _bebar_old(getMaterialPropertyOldByName<RankTwoTensor>(prependBaseName("be_bar"))),
    _bebar_det(declareADProperty<Real>(prependBaseName("be_bar_det"))),
    _F_old(getMaterialPropertyOldByName<RankTwoTensor>(prependBaseName("deformation_gradient"))),
    _F(getADMaterialPropertyByName<RankTwoTensor>(prependBaseName("deformation_gradient"))),
    _d_name(getVar("phase_field", 0)->name()),
    _psie_name(prependBaseName("strain_energy_density_corr", true)),
    _psie_corr(declareADProperty<Real>(_psie_name)),
    _psie_active_corr(declareADProperty<Real>(_psie_name + "_active")),
    _dpsie_dd_corr(declareADProperty<Real>(derivativePropertyName(_psie_name, {_d_name}))),
    _psie_unsplit(declareADProperty<Real>(_psie_name + "_unsplit")),
    _apply_strain_energy_split(getParam<bool>("apply_strain_energy_split"))
{
  _check_range = true;
}

void
LargeDeformationJ2PlasticityCorrection::setElasticityModel(
    LargeDeformationElasticityModel * elasticity_model)
{
  auto * cnh = dynamic_cast<CNHIsotropicElasticity *>(elasticity_model);
  if (!cnh)
    mooseError(type(), ": requires a CNHIsotropicElasticity model; got ", elasticity_model->type());
  _K = &cnh->getK();
  _G = &cnh->getG();
  _ge = &cnh->getDegradation();
  _dge_dd = &cnh->getDegradationDerivative();
  LargeDeformationJ2PlasticityBase::setElasticityModel(elasticity_model);
}

void
LargeDeformationJ2PlasticityCorrection::initQpStatefulProperties()
{
  _Fp[_qp].setToIdentity();
  _ep[_qp] = 0;
  _bebar[_qp].setToIdentity();

  if (!_recover)
    return;

  const std::vector<std::string> indices = {"x", "y", "z"};
  unsigned int qp_sel = QpMapping::getQP(_qp + 1, _lookup);

  _ep[_qp] = _solution_object_ptr->pointValue(
      _t, _current_elem->true_centroid(), "effective_plastic_strain_" + formatQP(qp_sel), nullptr);
  if (_ep[_qp] < 0)
    _ep[_qp] = 0;

  for (int i_ind = 0; i_ind < 3; i_ind++)
    for (int j_ind = 0; j_ind < 3; j_ind++)
      _bebar[_qp](i_ind, j_ind) = _solution_object_ptr->pointValue(
          _t,
          _current_elem->true_centroid(),
          "be_bar_" + indices[i_ind] + indices[j_ind] + "_" + formatQP(qp_sel),
          nullptr);

  // Re-enforce det(be_bar) = 1 on the mapped be_bar.
  const Real det_bebar = MetaPhysicL::raw_value(_bebar[_qp].det());
  if (std::abs(det_bebar - 1.0) > 1e-10)
    _bebar[_qp] /= std::cbrt(det_bebar);
}

void
LargeDeformationJ2PlasticityCorrection::updateState(ADRankTwoTensor & stress,
                                                    ADRankTwoTensor & /*Fe*/)
{
  using std::cbrt;
  using std::sqrt;

  ADRankTwoTensor I2;
  I2.setToIdentity();

  if (_recover && MetaPhysicL::raw_value(_ep[_qp]) < 0)
    _ep[_qp] = 0;

  ADReal delta_ep = 0;

  // Compute incremental fbar
  ADRankTwoTensor f = _F[_qp] * _F_old[_qp].inverse();
  ADRankTwoTensor fbar = f / cbrt(f.det());

  // Compute bebar_trial
  _bebar[_qp] = fbar * _bebar_old[_qp] * fbar.transpose();

  // Compute trial deviatoric stress and its norm
  ADRankTwoTensor s_trial = (*_G)[_qp] * (*_ge)[_qp] * _bebar[_qp].deviatoric();
  ADReal s_trial_norm = s_trial.doubleContraction(s_trial);
  if (MooseUtils::absoluteFuzzyEqual(s_trial_norm, 0))
    s_trial_norm.value() = libMesh::TOLERANCE * libMesh::TOLERANCE;
  s_trial_norm = sqrt(s_trial_norm);

  _Np[_qp] = sqrt(1.5) * s_trial / s_trial_norm;

  // Return mapping
  ADReal phi = computeResidual(s_trial_norm, delta_ep);
  if (phi > 0)
  {
    returnMappingSolve(s_trial_norm, delta_ep, _console);

    _ep[_qp] = _ep_old[_qp] + delta_ep;
    ADRankTwoTensor s = s_trial - (2.0 / 3.0) * (*_ge)[_qp] * (*_G)[_qp] * delta_ep *
                                      _bebar[_qp].trace() * _Np[_qp];

    ADReal J = _F[_qp].det();
    ADReal p = 0.5 * (*_K)[_qp] * (J * J - 1);
    ADRankTwoTensor tau = J >= 1.0 ? (*_ge)[_qp] * p * I2 + s : p * I2 + s;
    stress = tau / J;

    computeCorrectionTerm(s / (*_ge)[_qp] / (*_G)[_qp]);
  }
  else
  {
    _ep[_qp] = _ep_old[_qp];

    ADReal J = _F[_qp].det();
    ADReal p = 0.5 * (*_K)[_qp] * (J * J - 1);
    ADRankTwoTensor tau = J >= 1.0 ? (*_ge)[_qp] * p * I2 + s_trial : p * I2 + s_trial;
    stress = tau / J;
  }

  computeStrainEnergyDensity();
  _hardening_model->plasticEnergy(_ep[_qp]);
  _hardening_model->plasticDissipation(delta_ep, _ep[_qp], 0);

  if (_t_step > 0)
  {
    _heat[_qp] = _hardening_model->plasticDissipation(delta_ep, _ep[_qp], 1) * delta_ep / _dt;
    _heat[_qp] += _hardening_model->thermalConjugate(_ep[_qp]) * delta_ep / _dt;
  }
  else
    _heat[_qp] = 0;

  _bebar_det[_qp] = _bebar[_qp].det();
}

Real
LargeDeformationJ2PlasticityCorrection::computeReferenceResidual(
    const ADReal & effective_trial_stress, const ADReal & delta_ep)
{
  return MetaPhysicL::raw_value(effective_trial_stress - std::sqrt(2.0 / 3.0) * (*_ge)[_qp] *
                                                             (*_G)[_qp] * delta_ep *
                                                             _bebar[_qp].trace());
}

ADReal
LargeDeformationJ2PlasticityCorrection::computeResidual(const ADReal & effective_trial_stress,
                                                        const ADReal & delta_ep)
{
  ADReal ep = _ep_old[_qp] + delta_ep;
  return effective_trial_stress -
         std::sqrt(2.0 / 3.0) * (_hardening_model->plasticEnergy(ep, 1) +
                                 _hardening_model->plasticDissipation(delta_ep, ep, 1)) -
         std::sqrt(2.0 / 3.0) * (*_ge)[_qp] * (*_G)[_qp] * delta_ep * _bebar[_qp].trace();
}

ADReal
LargeDeformationJ2PlasticityCorrection::computeDerivative(const ADReal & /*effective_trial_stress*/,
                                                          const ADReal & delta_ep)
{
  ADReal ep = _ep_old[_qp] + delta_ep;
  return -std::sqrt(2.0 / 3.0) * (_hardening_model->plasticEnergy(ep, 2) +
                                  _hardening_model->plasticDissipation(delta_ep, ep, 2)) -
         std::sqrt(2.0 / 3.0) * (*_ge)[_qp] * (*_G)[_qp] * _bebar[_qp].trace();
}

void
LargeDeformationJ2PlasticityCorrection::computeCorrectionTerm(const ADRankTwoTensor & devbebar)
{
  using std::cbrt;
  using std::sqrt;

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
  ADReal U = 0.5 * (*_K)[_qp] * (0.5 * (J * J - 1) - log(J));
  ADReal W = 0.5 * (*_G)[_qp] * (_bebar[_qp].trace() - 3.0);

  _psie_unsplit[_qp] = U + W;

  ADReal E_el_pos = J >= 1.0 ? U + W : W;
  ADReal E_el_neg = J >= 1.0 ? 0.0 : U;

  _psie_active_corr[_qp] = _apply_strain_energy_split ? E_el_pos : _psie_unsplit[_qp];
  _psie_corr[_qp] = (*_ge)[_qp] * E_el_pos + E_el_neg;
  _dpsie_dd_corr[_qp] = (*_dge_dd)[_qp] * _psie_active_corr[_qp];
}
