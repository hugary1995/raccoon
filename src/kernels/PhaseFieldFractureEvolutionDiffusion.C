//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PhaseFieldFractureEvolutionDiffusion.h"

registerADMooseObject("raccoonApp", PhaseFieldFractureEvolutionDiffusion);

defineADLegacyParams(PhaseFieldFractureEvolutionDiffusion);

template <ComputeStage compute_stage>
InputParameters
PhaseFieldFractureEvolutionDiffusion<compute_stage>::validParams()
{
  auto params = ADKernelGrad<compute_stage>::validParams();
  params.addClassDescription("computes the diffusion term in phase-field evolution equation");
  params.addParam<MaterialPropertyName>("kappa_name", "kappa", "kappa name");
  params.addParam<MaterialPropertyName>("mobility_name", "M", "name of mobility");
  return params;
}

template <ComputeStage compute_stage>
PhaseFieldFractureEvolutionDiffusion<compute_stage>::PhaseFieldFractureEvolutionDiffusion(
    const InputParameters & parameters)
  : PhaseFieldFractureEvolutionBase<compute_stage>(parameters),
    _kappa(getMaterialProperty<Real>("kappa_name")),
    _M(getMaterialProperty<Real>("mobility_name"))
{
}

template <ComputeStage compute_stage>
ADRealVectorValue
PhaseFieldFractureEvolutionDiffusion<compute_stage>::precomputeQpResidual()
{
  return _M[_qp] * _kappa[_qp] * _grad_u[_qp];
}
