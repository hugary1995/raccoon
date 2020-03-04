//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CNHDegradedElasticPlasticPK1Stress_LinearHardening.h"

registerADMooseObject("raccoonApp", CNHDegradedElasticPlasticPK1Stress_LinearHardening);

defineADLegacyParams(CNHDegradedElasticPlasticPK1Stress_LinearHardening);

template <ComputeStage compute_stage>
InputParameters
CNHDegradedElasticPlasticPK1Stress_LinearHardening<compute_stage>::validParams()
{
  InputParameters params = CNHDegradedElasticPlasticPK1StressBase<compute_stage>::validParams();
  params.addClassDescription("defines a linear hardening law");
  params.addRequiredParam<Real>("yield_stress", "yield stress");
  params.addRequiredParam<Real>("slope", "slope of the hardening curve");
  return params;
}

template <ComputeStage compute_stage>
CNHDegradedElasticPlasticPK1Stress_LinearHardening<compute_stage>::
    CNHDegradedElasticPlasticPK1Stress_LinearHardening(const InputParameters & parameters)
  : CNHDegradedElasticPlasticPK1StressBase<compute_stage>(parameters),
    _yield_stress(getParam<Real>("yield_stress")),
    _k(getParam<Real>("slope"))
{
}

template <ComputeStage compute_stage>
ADReal
CNHDegradedElasticPlasticPK1Stress_LinearHardening<compute_stage>::H(ADReal ep)
{
  return _yield_stress * ep + 0.5 * _k * ep * ep;
}

template <ComputeStage compute_stage>
ADReal
CNHDegradedElasticPlasticPK1Stress_LinearHardening<compute_stage>::dH_dep(ADReal ep)
{
  return _yield_stress + _k * ep;
}

template <ComputeStage compute_stage>
ADReal CNHDegradedElasticPlasticPK1Stress_LinearHardening<compute_stage>::d2H_dep2(ADReal /*ep*/)
{
  return _k;
}
