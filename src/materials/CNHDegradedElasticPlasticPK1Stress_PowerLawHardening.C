//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CNHDegradedElasticPlasticPK1Stress_PowerLawHardening.h"

registerADMooseObject("raccoonApp", CNHDegradedElasticPlasticPK1Stress_PowerLawHardening);

InputParameters
CNHDegradedElasticPlasticPK1Stress_PowerLawHardening::validParams()
{
  InputParameters params = CNHDegradedElasticPlasticPK1StressBase::validParams();
  params.addClassDescription("defines a power law hardening");
  params.addRequiredParam<Real>("yield_stress", "yield stress");
  params.addRequiredParam<Real>("exponent", "exponent of the hardening curve");
  params.addRequiredParam<Real>("ep0", "characteristic plastic strain");
  return params;
}

CNHDegradedElasticPlasticPK1Stress_PowerLawHardening::
    CNHDegradedElasticPlasticPK1Stress_PowerLawHardening(const InputParameters & parameters)
  : CNHDegradedElasticPlasticPK1StressBase(parameters),
    _yield_stress(getParam<Real>("yield_stress")),
    _n(getParam<Real>("exponent")),
    _ep0(getParam<Real>("ep0"))
{
}

ADReal
CNHDegradedElasticPlasticPK1Stress_PowerLawHardening::H(ADReal ep)
{
  return _n * _yield_stress * _ep0 / (_n + 1) * (std::pow(1 + ep / _ep0, 1 / _n + 1) - 1);
}

ADReal
CNHDegradedElasticPlasticPK1Stress_PowerLawHardening::dH_dep(ADReal ep)
{
  return _gp * _yield_stress * std::pow(1 + ep / _ep0, 1 / _n);
}

ADReal
CNHDegradedElasticPlasticPK1Stress_PowerLawHardening::d2H_dep2(ADReal ep)
{
  return _gp * _yield_stress * std::pow(1 + ep / _ep0, 1 / _n - 1) / _n / _ep0;
}

ADReal
CNHDegradedElasticPlasticPK1Stress_PowerLawHardening::plastic_dissipation(ADReal ep)
{
  return _gp * _n * _yield_stress * _ep0 / (_n + 1) * (std::pow(1 + ep / _ep0, 1 / _n + 1) - 1);
}
