//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CNHDegradedElasticPlasticPK1Stress_LinearHardening_StrictlyDissipative.h"

registerADMooseObject("raccoonApp",
                      CNHDegradedElasticPlasticPK1Stress_LinearHardening_StrictlyDissipative);

InputParameters
CNHDegradedElasticPlasticPK1Stress_LinearHardening_StrictlyDissipative::validParams()
{
  InputParameters params = CNHDegradedElasticPlasticPK1StressBase::validParams();
  params.addClassDescription("defines a linear hardening law");
  params.addRequiredParam<Real>("yield_stress", "yield stress");
  params.addRequiredParam<Real>("slope", "slope of the hardening curve");
  return params;
}

CNHDegradedElasticPlasticPK1Stress_LinearHardening_StrictlyDissipative::
    CNHDegradedElasticPlasticPK1Stress_LinearHardening_StrictlyDissipative(
        const InputParameters & parameters)
  : CNHDegradedElasticPlasticPK1StressBase(parameters),
    _yield_stress(getParam<Real>("yield_stress")),
    _k(getParam<Real>("slope"))
{
}

ADReal CNHDegradedElasticPlasticPK1Stress_LinearHardening_StrictlyDissipative::H(ADReal /*ep*/)
{
  return 0;
}

ADReal
CNHDegradedElasticPlasticPK1Stress_LinearHardening_StrictlyDissipative::dH_dep(ADReal ep)
{
  return _yield_stress + _k * ep;
}

ADReal
    CNHDegradedElasticPlasticPK1Stress_LinearHardening_StrictlyDissipative::d2H_dep2(ADReal /*ep*/)
{
  return _k;
}

ADReal
CNHDegradedElasticPlasticPK1Stress_LinearHardening_StrictlyDissipative::plastic_dissipation(
    ADReal ep)
{
  return _yield_stress * ep + 0.5 * _k * ep * ep;
}
