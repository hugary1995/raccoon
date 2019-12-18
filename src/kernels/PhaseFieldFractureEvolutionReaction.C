//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PhaseFieldFractureEvolutionReaction.h"

registerADMooseObject("raccoonApp", PhaseFieldFractureEvolutionReaction);

defineADLegacyParams(PhaseFieldFractureEvolutionReaction);

template <ComputeStage compute_stage>
InputParameters
PhaseFieldFractureEvolutionReaction<compute_stage>::validParams()
{
  auto params = PhaseFieldFractureEvolutionBase<compute_stage>::validParams();
  params.addClassDescription("computes the diffusion term in phase-field evolution equation");
  return params;
}

template <ComputeStage compute_stage>
PhaseFieldFractureEvolutionReaction<compute_stage>::PhaseFieldFractureEvolutionReaction(
    const InputParameters & parameters)
  : PhaseFieldFractureEvolutionBase<compute_stage>(parameters)
{
}

template <ComputeStage compute_stage>
ADReal
PhaseFieldFractureEvolutionReaction<compute_stage>::computeQpResidual()
{
  // diffusion
  ADReal residual_diffusion = _M[_qp] * _kappa[_qp] * _grad_test[_i][_qp] * _grad_u[_qp];

  // reaction like driving force
  ADReal residual_driving =
      _test[_i][_qp] * (_dg_dd[_qp] * fractureDrivingEnergy() + _dw_dd[_qp] * _M[_qp]);

  return residual_diffusion + residual_driving;
}
