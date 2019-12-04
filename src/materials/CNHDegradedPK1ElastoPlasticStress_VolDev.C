//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CNHDegradedPK1ElastoPlasticStress_VolDev.h"

registerADMooseObject("raccoonApp", CNHDegradedPK1ElastoPlasticStress_VolDev);

defineADValidParams(
    CNHDegradedPK1ElastoPlasticStress_VolDev,
    ADDegradedStressBase,
    params.addClassDescription("Compute degraded stress following small deformation elasticity "
                               "with a volumetric-deviatoric active/inactive split");
    params.addRequiredParam<Real>("W0", "plastic work threshold.");
    params.addRequiredParam<Real>("yield_stress", "yield stress");
    params.addRequiredParam<Real>("linear_hardening_coefficient", "linear hardening coefficient"););

template <ComputeStage compute_stage>
CNHDegradedPK1ElastoPlasticStress_VolDev<compute_stage>::CNHDegradedPK1ElastoPlasticStress_VolDev(
    const InputParameters & parameters)
  : ADDegradedStressBase<compute_stage>(parameters),
    _deformation_gradient(
        getADMaterialProperty<RankTwoTensor>(_base_name + "deformation_gradient")),
    _be_bar(declareADProperty<RankTwoTensor>(_base_name + "volume_perserving_be")),
    _alpha(declareADProperty<Real>(_base_name + "alpha")),
    _deformation_gradient_old(
        getMaterialPropertyOldByName<RankTwoTensor>(_base_name + "deformation_gradient")),
    _be_bar_old(getMaterialPropertyOldByName<RankTwoTensor>(_base_name + "volume_perserving_be")),
    _alpha_old(getMaterialPropertyOldByName<Real>(_base_name + "alpha")),
    _yield_stress(getParam<Real>("yield_stress")),
    _k(getParam<Real>("linear_hardening_coefficient")),
    _plastic_strain(declareADProperty<RankTwoTensor>(_base_name + "plastic_strain")),
    _cauchy_stress(declareADProperty<RankTwoTensor>(_base_name + "cauchy_stress")),
    _Wp(declareADProperty<Real>("plastic_work")),
    _Wp_old(getMaterialPropertyOldByName<Real>("plastic_work")),
    _Wp_degraded(declareADProperty<Real>("degraded_plastic_work")),
    _W0(getParam<Real>("W0")),
    _E_el_degraded(declareADProperty<Real>("degraded_elastic_energy"))
{
}

template <ComputeStage compute_stage>
void
CNHDegradedPK1ElastoPlasticStress_VolDev<compute_stage>::initQpStatefulProperties()
{
  ADDegradedStressBase<compute_stage>::initQpStatefulProperties();
  _be_bar[_qp].zero();
  _be_bar[_qp].addIa(1.0);
  _alpha[_qp] = 0;
  _Wp[_qp] = 0;
}

template <ComputeStage compute_stage>
void
CNHDegradedPK1ElastoPlasticStress_VolDev<compute_stage>::computeQpStress()
{
  // isotropic elasticity is assumed and should be enforced
  const Real lambda = _elasticity_tensor[_qp](0, 0, 1, 1);
  const Real mu = _elasticity_tensor[_qp](0, 1, 0, 1);
  const Real K = lambda + 2.0 * mu / LIBMESH_DIM;

  // identity tensor
  ADRankTwoTensor I2;
  I2.addIa(1.0);

  // elastic and plastic degradation function
  ADReal g = _g[_qp];
  ADReal gp = _g[_qp];

  // update the current configuration
  ADRankTwoTensor f = _deformation_gradient[_qp] * (_deformation_gradient_old[_qp].inverse());

  // compute the damage/elastic predictor
  ADRankTwoTensor f_bar = std::pow(f.det(), -1.0 / 3.0) * f;
  ADRankTwoTensor be_bar_trial = f_bar * _be_bar_old[_qp] * (f_bar.transpose());
  ADRankTwoTensor s_trial = g * mu * be_bar_trial.deviatoric();

  // check for plastic loading
  ADRankTwoTensor s;
  ADReal yield_function_trial = std::sqrt(s_trial.doubleContraction(s_trial)) -
                                gp * std::sqrt(2.0 / 3.0) * (_yield_stress + _alpha_old[_qp] * _k);

  ADReal plastic_increment = 0;
  if (yield_function_trial <= 0.0)
  {
    s = s_trial;
    _alpha[_qp] = _alpha_old[_qp];
  }
  else
  {
    // return mapping
    ADReal mu_bar = g * mu * be_bar_trial.trace() / 3.0;
    plastic_increment = yield_function_trial / (2.0 / 3.0 * gp * _k + 2.0 * mu_bar);
    s = s_trial - 2 * mu_bar * plastic_increment *
                      (s_trial / (std::sqrt(s_trial.doubleContraction(s_trial))));
    _alpha[_qp] = _alpha_old[_qp] + std::sqrt(2.0 / 3.0) * plastic_increment;
  }

  // update the Kirchhoff stress
  ADReal J = _deformation_gradient[_qp].det();
  ADReal p = J >= 1.0 ? g * 0.5 * K * (J - 1.0 / J) : 0.5 * K * (J - 1.0 / J);
  ADRankTwoTensor tau = J * p * I2 + s;

  // update the intermediate configuration
  ADReal Ie_bar = be_bar_trial.trace() / 3.0;
  ADRankTwoTensor be_bar_dev = s / g / mu;
  ADReal a = be_bar_dev(0, 0);
  ADReal b = be_bar_dev(1, 1);
  ADReal c = be_bar_dev(2, 2);
  ADReal d = be_bar_dev(1, 2);
  ADReal e = be_bar_dev(0, 2);
  ADReal h = be_bar_dev(0, 1);

  ADReal C2 = a + b + c;
  ADReal C1 = a * b + a * c + b * c - d * d - e * e - h * h;
  ADReal C0 = a * b * c + 2.0 * d * e * h - a * d * d - b * e * e - c * h * h - 1.0;

  ADReal resid = Ie_bar * Ie_bar * Ie_bar + C2 * Ie_bar * Ie_bar + C1 * Ie_bar + C0;
  ADReal resid0 = resid;
  ADReal jacob, delta_I;
  int iter = 0;
  while (std::abs(resid) > 1E-12)
  {
    jacob = 3.0 * Ie_bar * Ie_bar + 2.0 * C2 * Ie_bar + C1;
    delta_I = -resid / jacob;
    Ie_bar = Ie_bar + delta_I;
    resid = Ie_bar * Ie_bar * Ie_bar + C2 * Ie_bar * Ie_bar + C1 * Ie_bar + C0;
    iter++;
    if (iter > 20)
    {
      std::cout << "resid0 = " << resid0 << std::endl;
      std::cout << "resid = " << resid << std::endl;
      be_bar_dev.print();
      mooseError("too many iterations in return mapping.");
    }
  }
  _be_bar[_qp] = be_bar_dev + Ie_bar * I2;
  _stress[_qp] = tau * _deformation_gradient[_qp].inverse().transpose();

  ADRankTwoTensor be = _be_bar[_qp] * std::pow(J, 2.0 / 3.0);
  ADRankTwoTensor Cp =
      _deformation_gradient[_qp].transpose() * be.inverse() * _deformation_gradient[_qp];

  _plastic_strain[_qp] = 0.5 * (Cp - I2);
  _elastic_strain[_qp] = _mechanical_strain[_qp] - _plastic_strain[_qp];
  _cauchy_stress[_qp] = tau / J;

  ADReal U = 0.5 * K * (0.5 * (J * J - 1) - std::log(J));
  ADReal W = 0.5 * mu * (_be_bar[_qp].trace() - 3.0);
  ADReal E_el_pos = J >= 1 ? U + W : W;
  ADReal E_el_neg = J >= 1 ? 0 : U;
  _E_el_degraded[_qp] = g * E_el_pos + E_el_neg;

  // enforce irreversibility using history approach
  if (_E_el_pos_old)
    E_el_pos = computeQpHistory(E_el_pos);

  _Wp[_qp] = _Wp_old[_qp] + plastic_increment * std::sqrt(s.doubleContraction(s));
  _Wp_degraded[_qp] = gp * _Wp[_qp];

  // store the positive elastic energy
  // note that it becomes the old value in the next step
  _E_el_pos[_qp] = E_el_pos;

  _E_driving[_qp] = _Wp[_qp] > _W0 ? E_el_pos + _Wp[_qp] - _W0 : E_el_pos;
}
