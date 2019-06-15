//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "DegradedCoefReaction.h"

registerADMooseObject("raccoonApp", DegradedCoefReaction);

defineADValidParams(DegradedCoefReaction,
                    ADCoefReaction,
                    params.addParam<MaterialPropertyName>(
                        "degradation_name",
                        "g",
                        "name of the material that holds the degradation function value"););

template <ComputeStage compute_stage>
DegradedCoefReaction<compute_stage>::DegradedCoefReaction(const InputParameters & parameters)
  : ADCoefReaction<compute_stage>(parameters), _g(adGetADMaterialProperty<Real>("degradation_name"))
{
}

template <ComputeStage compute_stage>
ADResidual
DegradedCoefReaction<compute_stage>::computeQpResidual()
{
  return _g[_qp] * ADCoefReaction<compute_stage>::computeQpResidual();
}
