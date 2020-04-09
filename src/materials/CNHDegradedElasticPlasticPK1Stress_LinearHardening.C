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
  params.addParam<Real>("W0", 0, "activation energy");
  return params;
}

template <ComputeStage compute_stage>
CNHDegradedElasticPlasticPK1Stress_LinearHardening<compute_stage>::
    CNHDegradedElasticPlasticPK1Stress_LinearHardening(const InputParameters & parameters)
  : CNHDegradedElasticPlasticPK1StressBase<compute_stage>(parameters),
    _yield_stress(getParam<Real>("yield_stress")),
    _k(getParam<Real>("slope")),
    _W0(getParam<Real>("W0"))
{
}

template <ComputeStage compute_stage>
ADReal
CNHDegradedElasticPlasticPK1Stress_LinearHardening<compute_stage>::H(ADReal ep)
{
  return Macaulay(_yield_stress * ep + 0.5 * _k * ep * ep - _W0);
}

template <ComputeStage compute_stage>
ADReal
CNHDegradedElasticPlasticPK1Stress_LinearHardening<compute_stage>::dH_dep(ADReal ep)
{
  return _gp * (_yield_stress + _k * ep);
}

template <ComputeStage compute_stage>
ADReal CNHDegradedElasticPlasticPK1Stress_LinearHardening<compute_stage>::d2H_dep2(ADReal /*ep*/)
{
  return _gp * _k;
}

template <ComputeStage compute_stage>
ADReal
CNHDegradedElasticPlasticPK1Stress_LinearHardening<compute_stage>::plastic_dissipation(ADReal ep)
{
  return _gp * _yield_stress * ep;
}
