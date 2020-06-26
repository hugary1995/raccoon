//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CNHDegradedElasticPlasticPK1StressBase.h"

InputParameters
CNHDegradedElasticPlasticPK1StressBase::validParams()
{
  InputParameters params = ADDegradedElasticStressBase::validParams();
  params.addClassDescription(
      "computes elastic and plastic stress for a compressible Neo-Hookean material");
  params.addParam<MaterialPropertyName>(
      "plastic_degradation_name",
      "gp",
      "name of the plastic degradation material. Use plastic_degradation_mat instead.");
  params.addParam<MaterialPropertyName>("plastic_degradation_mat",
                                        "name of the material that holds the plastic degradation");
  params.addParam<UserObjectName>("plastic_degradation_uo",
                                  "name of the userobject that holds the plastic degradation");
  params.addParam<MaterialPropertyName>(
      "plastic_work_name", "W_pl", "name of the material for plastic work");
  params.addParam<bool>(
      "legacy_plastic_work", false, "whether to use the deprecated plastic work calculation");
  params.addParam<bool>(
      "enforce_isochoricity", true, "whether to enforce the isochoricity constraint");
  params.addParam<bool>("use_cauchy_stress",
                        false,
                        "use PK1 stress by default, use cauchy stress if set to true in the stress "
                        "divergence kernel");
  return params;
}

CNHDegradedElasticPlasticPK1StressBase::CNHDegradedElasticPlasticPK1StressBase(
    const InputParameters & parameters)
  : ADDegradedElasticStressBase(parameters),
    _deformation_gradient(
        getADMaterialPropertyByName<RankTwoTensor>(_base_name + "deformation_gradient")),
    _deformation_gradient_old(
        getMaterialPropertyOldByName<RankTwoTensor>(_base_name + "deformation_gradient")),
    _deformation_gradient_increment(
        getADMaterialPropertyByName<RankTwoTensor>(_base_name + "deformation_gradient_increment")),
    _be_bar(declareADProperty<RankTwoTensor>(_base_name + "volume_perserving_be")),
    _be_bar_old(getMaterialPropertyOldByName<RankTwoTensor>(_base_name + "volume_perserving_be")),
    _ep(declareADProperty<Real>(_base_name + "effective_plastic_strain")),
    _ep_old(getMaterialPropertyOldByName<Real>(_base_name + "effective_plastic_strain")),
    _plastic_strain(declareADProperty<RankTwoTensor>(_base_name + "plastic_strain")),
    _cauchy_stress(declareADProperty<RankTwoTensor>(_base_name + "cauchy_stress")),
    _pk1_stress(declareADProperty<RankTwoTensor>(_base_name + "pk1_stress")),
    _legacy(getParam<bool>("legacy_plastic_work")),
    _isochoricity(getParam<bool>("enforce_isochoricity")),
    _use_cauchy_stress(getParam<bool>("use_cauchy_stress")),
    _g_plastic_mat(isParamValid("plastic_degradation_mat")
                       ? &getADMaterialProperty<Real>("plastic_degradation_mat")
                       : &getADMaterialProperty<Real>("plastic_degradation_name")),
    _g_plastic_uo(isParamValid("plastic_degradation_uo")
                      ? &getUserObject<ADMaterialPropertyUserObject>("plastic_degradation_uo")
                      : nullptr),
    _W_pl_name(getParam<MaterialPropertyName>("plastic_work_name")),
    _W_pl(declareADProperty<Real>(_W_pl_name)),
    _W_pl_old(_legacy ? &getMaterialPropertyOldByName<Real>(_W_pl_name) : nullptr),
    _W_pl_degraded(declareADProperty<Real>(_W_pl_name + "_degraded")),
    _E_el_degraded(declareADProperty<Real>(_E_el_name + "_degraded"))
{
  if (parameters.isParamSetByUser("plastic_degradation_name"))
    mooseDeprecated("plastic_degradation_name is deprecated in favor of plastic_degradation_mat.");
  if (!_g_plastic_uo && parameters.isParamSetByAddParam("plastic_degradation_name"))
    mooseDeprecated("plastic_degradation_name is deprecated in favor of plastic_degradation_mat.");

  bool provided_by_mat = _g_plastic_mat;
  bool provided_by_uo = _g_plastic_uo;

  /// degradation should be provided
  if (!provided_by_mat && !provided_by_uo)
    mooseError("no degradation provided.");

  /// degradation should not be multiply defined
  if ((provided_by_mat ? 1 : 0) + (provided_by_uo ? 1 : 0) > 1)
    mooseError("degradation multiply defined.");
}

ADReal
CNHDegradedElasticPlasticPK1StressBase::gp()
{
  if (_g_plastic_mat)
    return (*_g_plastic_mat)[_qp];
  else if (_g_plastic_uo)
    return _g_plastic_uo->getData(_current_elem, _qp);
  else
    mooseError("Internal Error");

  return 0;
}

void
CNHDegradedElasticPlasticPK1StressBase::initQpStatefulProperties()
{
  ADDegradedElasticStressBase::initQpStatefulProperties();
  _be_bar[_qp].zero();
  _be_bar[_qp].addIa(1.0);
  _ep[_qp] = 0;
  _W_pl[_qp] = 0;
}

void
CNHDegradedElasticPlasticPK1StressBase::computeQpStress()
{
  // isotropic elasticity is assumed and should be enforced
  const ADReal lambda = _elasticity_tensor[_qp](0, 0, 1, 1);
  _G = _elasticity_tensor[_qp](0, 1, 0, 1);
  _K = lambda + 2.0 * _G / LIBMESH_DIM;

  computeQpDegradation();

  updateIntermediateConfiguration();

  returnMapping();

  updateCurrentConfiguration();

  computeFractureDrivingEnergy();
}

void
CNHDegradedElasticPlasticPK1StressBase::computeQpDegradation()
{
  _ge = g();
  _gp = gp();
}

void
CNHDegradedElasticPlasticPK1StressBase::updateIntermediateConfiguration()
{
  // incremental deformation gradient
  _f = _deformation_gradient_increment[_qp];
  _f_bar = _f / std::cbrt(_f.det());

  // compute the damage/elastic predictor
  _plastic_increment = 0;
  _be_bar_trial = _f_bar * _be_bar_old[_qp] * (_f_bar.transpose());
  _s_trial = _ge * _G * _be_bar_trial.deviatoric();
  _s_trial_norm = std::sqrt(_s_trial.doubleContraction(_s_trial));
  _np_trial = std::sqrt(1.5) * _s_trial / _s_trial_norm;
}

void
CNHDegradedElasticPlasticPK1StressBase::returnMapping()
{
  // check for plastic loading
  ADReal yield_function_trial = _s_trial_norm - std::sqrt(2.0 / 3.0) * dH_dep(_ep_old[_qp]);

  if (yield_function_trial <= 0.0)
  {
    _s = _s_trial;
    _ep[_qp] = _ep_old[_qp];
    return;
  }

  // return mapping
  ADReal yield_function_trial_initial = yield_function_trial;
  ADReal jacob, step;
  int iter = 0;
  while (std::abs(yield_function_trial) > 1E-06)
  {
    jacob = -std::sqrt(3.0 / 2.0) * _ge * _G * _be_bar_trial.trace() -
            std::sqrt(2.0 / 3.0) * d2H_dep2(_ep_old[_qp] + _plastic_increment);
    step = -yield_function_trial / jacob;
    _plastic_increment = _plastic_increment + step;
    yield_function_trial =
        _s_trial_norm -
        std::sqrt(3.0 / 2.0) * _ge * _G * _plastic_increment * _be_bar_trial.trace() -
        std::sqrt(2.0 / 3.0) * dH_dep(_ep_old[_qp] + _plastic_increment);
    iter++;
    if (iter > 50)
    {
      std::cout << "yield_function_trial_initial = " << yield_function_trial_initial << std::endl;
      std::cout << "yield_function_trial = " << yield_function_trial << std::endl;
      mooseWarning("too many iterations in return mapping.");
      break;
    }
  }

  _s = _s_trial - _ge * _G * _plastic_increment * _be_bar_trial.trace() * _np_trial;
  _ep[_qp] = _ep_old[_qp] + _plastic_increment;
}

void
CNHDegradedElasticPlasticPK1StressBase::updateCurrentConfiguration()
{
  // Identity tensor
  ADRankTwoTensor I2(RankTwoTensorTempl<ADReal>::initIdentity);

  // update the Kirchhoff stress
  _J = _deformation_gradient[_qp].det();
  ADReal p = 0.5 * _K * (_J * _J - 1.0);
  _tau = _J >= 1.0 ? _ge * p * I2 + _s : p * I2 + _s;
  _cauchy_stress[_qp] = _tau / _J;
  _pk1_stress[_qp] = _tau * _deformation_gradient[_qp].inverse().transpose();
  _stress[_qp] = _use_cauchy_stress ? _cauchy_stress[_qp] : _pk1_stress[_qp];

  if (_isochoricity)
    enforceIsochoricity();
  else
    _be_bar[_qp] = _be_bar_trial;

  _be = _be_bar[_qp] * std::pow(_J, 2.0 / 3.0);
  _Cp = _deformation_gradient[_qp].transpose() * _be.inverse() * _deformation_gradient[_qp];

  _plastic_strain[_qp] = 0.5 * (_Cp - I2);
  _elastic_strain[_qp] = _mechanical_strain[_qp] - _plastic_strain[_qp];
}

void
CNHDegradedElasticPlasticPK1StressBase::enforceIsochoricity()
{
  // Identity tensor
  ADRankTwoTensor I2(RankTwoTensorTempl<ADReal>::initIdentity);

  ADReal Ie_bar = _be_bar_trial.trace() / 3.0;
  _be_bar_dev = _s / _ge / _G;
  ADReal a = _be_bar_dev(0, 0);
  ADReal b = _be_bar_dev(1, 1);
  ADReal c = _be_bar_dev(2, 2);
  ADReal d = _be_bar_dev(1, 2);
  ADReal e = _be_bar_dev(0, 2);
  ADReal h = _be_bar_dev(0, 1);

  ADReal C2 = a + b + c;
  ADReal C1 = a * b + a * c + b * c - d * d - e * e - h * h;
  ADReal C0 = a * b * c + 2.0 * d * e * h - a * d * d - b * e * e - c * h * h - 1.0;

  ADReal resid = Ie_bar * Ie_bar * Ie_bar + C2 * Ie_bar * Ie_bar + C1 * Ie_bar + C0;
  ADReal resid0 = resid;
  ADReal jacob, delta_I;
  int iter = 0;
  while (std::abs(resid) > 1E-06)
  {
    jacob = 3.0 * Ie_bar * Ie_bar + 2.0 * C2 * Ie_bar + C1;
    delta_I = -resid / jacob;
    Ie_bar = Ie_bar + delta_I;
    resid = Ie_bar * Ie_bar * Ie_bar + C2 * Ie_bar * Ie_bar + C1 * Ie_bar + C0;
    iter++;
    if (iter > 50)
    {
      std::cout << "resid0 = " << resid0 << std::endl;
      std::cout << "resid = " << resid << std::endl;
      mooseWarning("too many iterations in enforceIsochoricity().");
      break;
    }
  }
  _be_bar[_qp] = _be_bar_dev + Ie_bar * I2;
}

void
CNHDegradedElasticPlasticPK1StressBase::computeFractureDrivingEnergy()
{
  ADReal U = 0.5 * _K * (0.5 * (_J * _J - 1) - std::log(_J));
  ADReal W = 0.5 * _G * (_be_bar[_qp].trace() - 3.0);
  ADReal E_el_pos = _J >= 1.0 ? U + W : W;
  ADReal E_el_neg = _J >= 1.0 ? 0.0 : U;

  _E_el_active[_qp] = E_el_pos;
  _E_el_degraded[_qp] = _ge * E_el_pos + E_el_neg;

  // plastic work
  if (_legacy)
    _W_pl[_qp] =
        (*_W_pl_old)[_qp] + _plastic_increment * std::sqrt(_s.doubleContraction(_s) + 1e-12);
  else
    _W_pl[_qp] = H(_ep[_qp]);

  _W_pl_degraded[_qp] = plastic_dissipation(_ep[_qp]);
}
