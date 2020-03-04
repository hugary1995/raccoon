//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PhaseFieldFractureEvolutionBarrier.h"

registerADMooseObject("raccoonApp", PhaseFieldFractureEvolutionBarrier);

defineADLegacyParams(PhaseFieldFractureEvolutionBarrier);

template <ComputeStage compute_stage>
InputParameters
PhaseFieldFractureEvolutionBarrier<compute_stage>::validParams()
{
  InputParameters params = ADKernelValue<compute_stage>::validParams();
  params.addClassDescription("computes the reaction term in phase-field evolution equation");
  params.addParam<MaterialPropertyName>("mobility_name", "mobility", "name of mobility");
  params.addParam<MaterialPropertyName>("local_dissipation_name", "w", "name of local dissipation");
  return params;
}

template <ComputeStage compute_stage>
PhaseFieldFractureEvolutionBarrier<compute_stage>::PhaseFieldFractureEvolutionBarrier(
    const InputParameters & parameters)
  : ADKernelValue<compute_stage>(parameters),
    _M(getMaterialProperty<Real>("mobility_name")),
    _dw_dd(getADMaterialProperty<Real>(derivativePropertyNameFirst(
        getParam<MaterialPropertyName>("local_dissipation_name"), _var.name())))
{
}

template <ComputeStage compute_stage>
ADReal
PhaseFieldFractureEvolutionBarrier<compute_stage>::precomputeQpResidual()
{
  return _dw_dd[_qp] * _M[_qp];
}
