//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CNHDegradedElasticPlasticPK1StressThreeField_LinearHardening.h"

registerADMooseObject("raccoonApp", CNHDegradedElasticPlasticPK1StressThreeField_LinearHardening);

InputParameters
CNHDegradedElasticPlasticPK1StressThreeField_LinearHardening::validParams()
{
  InputParameters params = CNHDegradedElasticPlasticPK1StressThreeFieldBase::validParams();
  params.addClassDescription("defines a linear hardening law");
  params.addRequiredParam<Real>("yield_stress", "yield stress");
  params.addRequiredParam<Real>("slope", "slope of the hardening curve");
  params.addParam<Real>("W0", 0, "activation energy");
  return params;
}

CNHDegradedElasticPlasticPK1StressThreeField_LinearHardening::
    CNHDegradedElasticPlasticPK1StressThreeField_LinearHardening(const InputParameters & parameters)
  : CNHDegradedElasticPlasticPK1StressThreeFieldBase(parameters),
    _yield_stress(getParam<Real>("yield_stress")),
    _k(getParam<Real>("slope")),
    _W0(getParam<Real>("W0"))
{
}

ADReal
CNHDegradedElasticPlasticPK1StressThreeField_LinearHardening::H(ADReal ep)
{
  return Macaulay(_yield_stress * ep + 0.5 * _k * ep * ep - _W0);
}

ADReal
CNHDegradedElasticPlasticPK1StressThreeField_LinearHardening::dH_dep(ADReal ep)
{
  return _gp * (_yield_stress + _k * ep);
}

ADReal CNHDegradedElasticPlasticPK1StressThreeField_LinearHardening::d2H_dep2(ADReal /*ep*/)
{
  return _gp * _k;
}

ADReal
CNHDegradedElasticPlasticPK1StressThreeField_LinearHardening::plastic_dissipation(ADReal ep)
{
  return _gp * _yield_stress * ep;
}
