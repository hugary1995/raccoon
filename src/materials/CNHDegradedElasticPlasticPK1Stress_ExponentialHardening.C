//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CNHDegradedElasticPlasticPK1Stress_ExponentialHardening.h"

registerADMooseObject("raccoonApp", CNHDegradedElasticPlasticPK1Stress_ExponentialHardening);

InputParameters
CNHDegradedElasticPlasticPK1Stress_ExponentialHardening::validParams()
{
  InputParameters params = CNHDegradedElasticPlasticPK1StressBase::validParams();
  params.addClassDescription("defines a exponential hardening law $\\sigma_\\infty - "
                             "(\\sigma_\\infty - \\sigma_Y)\\exp(-\\delta \\plasticstrain)$");
  params.addRequiredParam<Real>("yield_stress", "yield stress");
  params.addRequiredParam<Real>("ultimate_stress", "ultimate stress");
  params.addRequiredParam<Real>("delta", "hardening rate");
  return params;
}

CNHDegradedElasticPlasticPK1Stress_ExponentialHardening::
    CNHDegradedElasticPlasticPK1Stress_ExponentialHardening(const InputParameters & parameters)
  : CNHDegradedElasticPlasticPK1StressBase(parameters),
    _yield_stress(getParam<Real>("yield_stress")),
    _ultimate_stress(getParam<Real>("ultimate_stress")),
    _delta(getParam<Real>("delta"))
{
}

ADReal
CNHDegradedElasticPlasticPK1Stress_ExponentialHardening::H(ADReal ep)
{
  return _ultimate_stress * ep +
         (_ultimate_stress - _yield_stress) / _delta * std::exp(-_delta * ep) -
         (_ultimate_stress - _yield_stress) / _delta;
}

ADReal
CNHDegradedElasticPlasticPK1Stress_ExponentialHardening::dH_dep(ADReal ep)
{
  ADReal R = _ultimate_stress - (_ultimate_stress - _yield_stress) * std::exp(-_delta * ep);
  return _gp * R;
}

ADReal
CNHDegradedElasticPlasticPK1Stress_ExponentialHardening::d2H_dep2(ADReal ep)
{
  ADReal dR_dep = (_ultimate_stress - _yield_stress) * std::exp(-_delta * ep) * _delta;
  return _gp * dR_dep;
}

ADReal
CNHDegradedElasticPlasticPK1Stress_ExponentialHardening::plastic_dissipation(ADReal ep)
{
  ADReal H = _ultimate_stress * ep +
             (_ultimate_stress - _yield_stress) / _delta * std::exp(-_delta * ep) -
             (_ultimate_stress - _yield_stress) / _delta;
  return _gp * H;
}
