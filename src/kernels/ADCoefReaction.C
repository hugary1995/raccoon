//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADCoefReaction.h"

registerADMooseObject("raccoonApp", ADCoefReaction);

defineADValidParams(ADCoefReaction,
                    ADKernel,
                    params.addParam<Real>("coefficient", 1.0, "Coefficient of the term"););

template <ComputeStage compute_stage>
ADCoefReaction<compute_stage>::ADCoefReaction(const InputParameters & parameters)
  : ADKernel<compute_stage>(parameters), _coef(getParam<Real>("coefficient"))
{
}

template <ComputeStage compute_stage>
ADReal
ADCoefReaction<compute_stage>::computeQpResidual()
{
  return _coef * _u[_qp];
}

adBaseClass(ADCoefReaction);
