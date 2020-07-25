//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CNHDegradedElasticPlasticPK1Stress_LinearHardening_Coalescence.h"

registerADMooseObject("raccoonApp", CNHDegradedElasticPlasticPK1Stress_LinearHardening_Coalescence);

InputParameters
CNHDegradedElasticPlasticPK1Stress_LinearHardening_Coalescence::validParams()
{
  InputParameters params = CNHDegradedElasticPlasticPK1Stress_LinearHardening::validParams();
  params.addClassDescription("defines a linear hardening law");
  params.addParam<MaterialPropertyName>(
      "local_dissipation_name", "w", "name of the local dissipation function");
  params.addRequiredParam<Real>("beta", "coalescence coefficient");
  params.addRequiredParam<Real>("e0", "characteristic plastic strain");
  params.addParam<MaterialPropertyName>(
      "base_mobility_name", "mobility", "name of the material that holds the base mobility");
  return params;
}

CNHDegradedElasticPlasticPK1Stress_LinearHardening_Coalescence::
    CNHDegradedElasticPlasticPK1Stress_LinearHardening_Coalescence(
        const InputParameters & parameters)
  : CNHDegradedElasticPlasticPK1Stress_LinearHardening(parameters),
    _w(getADMaterialProperty<Real>("local_dissipation_name")),
    _beta(getParam<Real>("beta")),
    _e0(getParam<Real>("e0")),
    _M0(getADMaterialProperty<Real>("base_mobility_name"))
{
}

ADReal
CNHDegradedElasticPlasticPK1Stress_LinearHardening_Coalescence::dH_dep(ADReal ep)
{
  return _gp * (_yield_stress + _k * ep) -
         (1 - _beta) / _e0 * _M0[_qp] * std::exp(-ep / _e0) * _w[_qp];
}

ADReal
CNHDegradedElasticPlasticPK1Stress_LinearHardening_Coalescence::d2H_dep2(ADReal ep)
{
  return _gp * _k + (1 - _beta) / _e0 / _e0 * _M0[_qp] * std::exp(-ep / _e0) * _w[_qp];
}
