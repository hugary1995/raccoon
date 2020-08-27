//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CNHDegradedElasticPlasticPK1StressThreeFieldBase.h"
#include "Assembly.h"

InputParameters
CNHDegradedElasticPlasticPK1StressThreeFieldBase::validParams()
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
      "enforce_isochoricity", true, "whether to enforce the isochoricity constraint");
  params.addParam<bool>("use_cauchy_stress",
                        false,
                        "use PK1 stress by default, use cauchy stress if set to true in the stress "
                        "divergence kernel");
  return params;
}

CNHDegradedElasticPlasticPK1StressThreeFieldBase::CNHDegradedElasticPlasticPK1StressThreeFieldBase(
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
    _isochoricity(getParam<bool>("enforce_isochoricity")),
    _use_cauchy_stress(getParam<bool>("use_cauchy_stress")),
    _g_plastic_mat(isParamValid("plastic_degradation_mat")
                       ? &getADMaterialProperty<Real>("plastic_degradation_mat")
                       : nullptr),
    _g_plastic_uo(isParamValid("plastic_degradation_uo")
                      ? &getUserObject<ADMaterialPropertyUserObject>("plastic_degradation_uo")
                      : nullptr),
    _W_pl_name(getParam<MaterialPropertyName>("plastic_work_name")),
    _W_pl(declareADProperty<Real>(_W_pl_name)),
    _W_pl_degraded(declareADProperty<Real>(_W_pl_name + "_degraded")),
    _E_el_degraded(declareADProperty<Real>(_E_el_name + "_degraded")),
    _current_elem_volume(_assembly.elemVolume())
{
  if (!_g_plastic_mat && !_g_plastic_uo)
  {
    _g_plastic_mat = &getADMaterialProperty<Real>("plastic_degradation_name");
    mooseDeprecated("plastic_degradation_name is deprecated in favor of plastic_degradation_mat.");
  }

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
CNHDegradedElasticPlasticPK1StressThreeFieldBase::gp()
{
  if (_g_plastic_mat)
    return (*_g_plastic_mat)[_qp];
  else if (_g_plastic_uo)
    return _g_plastic_uo->getRawData(_current_elem, _qp);
  else
    mooseError("Internal Error");

  return 0;
}

void
CNHDegradedElasticPlasticPK1StressThreeFieldBase::initQpStatefulProperties()
{
  ADDegradedElasticStressBase::initQpStatefulProperties();
  _be_bar[_qp].zero();
  _be_bar[_qp].addIa(1.0);
  _ep[_qp] = 0;
  _W_pl[_qp] = 0;
}

void
CNHDegradedElasticPlasticPK1StressThreeFieldBase::computeProperties()
{
  // piece-wise constant fields Theta and pressure
  _Theta = 0;
  _pressure = 0;

  // Theta
  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
    _Theta += _deformation_gradient[_qp].det() * _JxW[_qp] * _coord[_qp];
  _Theta /= _current_elem_volume;

  // pressure
  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    const ADReal lambda = _elasticity_tensor[_qp](0, 0, 1, 1);
    _G = _elasticity_tensor[_qp](0, 1, 0, 1);
    _K = lambda + 2.0 * _G / LIBMESH_DIM;
    _pressure += 0.5 * _K * (_Theta * _Theta - 1.0) * _JxW[_qp] * _coord[_qp];
  }
  _pressure /= _current_elem_volume;

  // stress
  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
    computeQpStress();
}

void
CNHDegradedElasticPlasticPK1StressThreeFieldBase::computeQpStress()
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
CNHDegradedElasticPlasticPK1StressThreeFieldBase::computeQpDegradation()
{
  _ge = g();
  _gp = gp();
}

void
CNHDegradedElasticPlasticPK1StressThreeFieldBase::updateIntermediateConfiguration()
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
CNHDegradedElasticPlasticPK1StressThreeFieldBase::returnMapping()
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
  while (std::abs(yield_function_trial) > 1E-06 &&
         std::abs(yield_function_trial) > 1e-06 * std::abs(yield_function_trial_initial))
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
CNHDegradedElasticPlasticPK1StressThreeFieldBase::updateCurrentConfiguration()
{
  // Identity tensor
  ADRankTwoTensor I2(RankTwoTensorTempl<ADReal>::initIdentity);

  // update the Kirchhoff stress
  _J = _deformation_gradient[_qp].det();
  _tau = _Theta >= 1.0 ? _ge * _pressure * I2 + _s : _pressure * I2 + _s;
  _cauchy_stress[_qp] = _tau / _J;
  _pk1_stress[_qp] = _tau * _deformation_gradient[_qp].inverse().transpose();
  _stress[_qp] = _use_cauchy_stress ? _cauchy_stress[_qp] : _pk1_stress[_qp];

  if (_isochoricity && _t_step > 1)
    enforceIsochoricity();
  else
    _be_bar[_qp] = _be_bar_trial;

  _be = _be_bar[_qp] * std::pow(_J, 2.0 / 3.0);
  _Cp = _deformation_gradient[_qp].transpose() * _be.inverse() * _deformation_gradient[_qp];

  _plastic_strain[_qp] = 0.5 * (_Cp - I2);
  _elastic_strain[_qp] = _mechanical_strain[_qp] - _plastic_strain[_qp];
}

void
CNHDegradedElasticPlasticPK1StressThreeFieldBase::enforceIsochoricity()
{
  // Identity tensor
  ADRankTwoTensor I2(RankTwoTensorTempl<ADReal>::initIdentity);

  _be_bar_dev = _s / _ge / _G;
  Real a = MetaPhysicL::raw_value(_be_bar_dev(0, 0));
  Real b = MetaPhysicL::raw_value(_be_bar_dev(1, 1));
  Real c = MetaPhysicL::raw_value(_be_bar_dev(2, 2));
  Real d = MetaPhysicL::raw_value(_be_bar_dev(1, 2));
  Real e = MetaPhysicL::raw_value(_be_bar_dev(0, 2));
  Real h = MetaPhysicL::raw_value(_be_bar_dev(0, 1));

  Real A = a + b + c;
  Real B = a * b + a * c + b * c - d * d - e * e - h * h;
  Real C = a * b * c + 2.0 * d * e * h - a * d * d - b * e * e - c * h * h - 1.0;

  // Real D = std::cbrt(-2 * A * A * A +
  //                    3 * std::sqrt(3) *
  //                        std::sqrt(4 * A * A * A * C - A * A * B * B - 18 * A * B * C +
  //                                  4 * B * B * B + 27 * C * C) +
  //                    9 * A * B - 27 * C);
  //
  // ADReal Ie_bar = D / 3 / std::cbrt(2) - std::cbrt(2) * (3 * B - A * A) / 3 / D - A / 3;

  Real Ie_bar = MetaPhysicL::raw_value(_be_bar_trial.trace() / 3.0);
  Real resid = Ie_bar * Ie_bar * Ie_bar + A * Ie_bar * Ie_bar + B * Ie_bar + C;
  Real resid0 = resid;
  Real jacob, delta_I;
  int iter = 0;
  while (std::abs(resid) > 1E-10 * std::abs(resid0) && std::abs(resid) > 1e-12)
  {
    jacob = 3.0 * Ie_bar * Ie_bar + 2.0 * A * Ie_bar + B;
    delta_I = -resid / jacob;
    Ie_bar = Ie_bar + delta_I;
    resid = Ie_bar * Ie_bar * Ie_bar + A * Ie_bar * Ie_bar + B * Ie_bar + C;
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
CNHDegradedElasticPlasticPK1StressThreeFieldBase::computeFractureDrivingEnergy()
{
  ADReal U = 0.5 * _K * (0.5 * (_J * _J - 1) - std::log(_J));
  ADReal W = 0.5 * _G * (_be_bar[_qp].trace() - 3.0);
  ADReal E_el_pos = _J >= 1.0 ? U + W : W;
  ADReal E_el_neg = _J >= 1.0 ? 0.0 : U;

  _E_el_active[_qp] = E_el_pos;
  _E_el_degraded[_qp] = _ge * E_el_pos + E_el_neg;

  _W_pl[_qp] = H(_ep[_qp]);

  _W_pl_degraded[_qp] = plastic_dissipation(_ep[_qp]);
}
