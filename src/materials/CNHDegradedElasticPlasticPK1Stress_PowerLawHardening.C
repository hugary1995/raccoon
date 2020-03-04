//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CNHDegradedElasticPlasticPK1Stress_PowerLawHardening.h"

registerADMooseObject("raccoonApp", CNHDegradedElasticPlasticPK1Stress_PowerLawHardening);

defineADLegacyParams(CNHDegradedElasticPlasticPK1Stress_PowerLawHardening);

template <ComputeStage compute_stage>
InputParameters
CNHDegradedElasticPlasticPK1Stress_PowerLawHardening<compute_stage>::validParams()
{
  InputParameters params = CNHDegradedElasticPlasticPK1StressBase<compute_stage>::validParams();
  params.addClassDescription("defines a linear hardening law");
  params.addRequiredParam<Real>("yield_stress", "yield stress");
  params.addRequiredParam<Real>("m", "multiplier");
  params.addRequiredParam<Real>("n", "exponent");
  return params;
}

template <ComputeStage compute_stage>
CNHDegradedElasticPlasticPK1Stress_PowerLawHardening<compute_stage>::
    CNHDegradedElasticPlasticPK1Stress_PowerLawHardening(const InputParameters & parameters)
  : CNHDegradedElasticPlasticPK1StressBase<compute_stage>(parameters),
    _yield_stress(getParam<Real>("yield_stress")),
    _m(getParam<Real>("m")),
    _n(getParam<Real>("n"))
{
}

template <ComputeStage compute_stage>
ADReal
CNHDegradedElasticPlasticPK1Stress_PowerLawHardening<compute_stage>::H(ADReal ep)
{
  return _n / (_n + 1.0) * _yield_stress / _m * (std::pow(1.0 + _m * ep, 1.0 / _n + 1.0) - 1.0);
}

template <ComputeStage compute_stage>
ADReal
CNHDegradedElasticPlasticPK1Stress_PowerLawHardening<compute_stage>::dH_dep(ADReal ep)
{
  return _yield_stress * std::pow(1.0 + _m * ep, 1.0 / _n);
}

template <ComputeStage compute_stage>
ADReal
CNHDegradedElasticPlasticPK1Stress_PowerLawHardening<compute_stage>::d2H_dep2(ADReal ep)
{
  return _m / _n * _yield_stress * std::pow(1.0 + _m * ep, 1.0 / _n - 1.0);
}
