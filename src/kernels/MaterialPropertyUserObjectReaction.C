//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "MaterialPropertyUserObjectReaction.h"

registerADMooseObject("raccoonApp", MaterialPropertyUserObjectReaction);

defineADLegacyParams(MaterialPropertyUserObjectReaction);

template <ComputeStage compute_stage>
InputParameters
MaterialPropertyUserObjectReaction<compute_stage>::validParams()
{
  InputParameters params = ADKernelValue<compute_stage>::validParams();
  params.addClassDescription(
      "Reaction term optionally multiplied with a material property stored in a user object");
  params.addRequiredParam<UserObjectName>("uo_name", "userobject that has values at qps");
  return params;
}

template <ComputeStage compute_stage>
MaterialPropertyUserObjectReaction<compute_stage>::MaterialPropertyUserObjectReaction(
    const InputParameters & parameters)
  : ADKernelValue<compute_stage>(parameters),
    _uo(getUserObject<MaterialPropertyUserObject>("uo_name"))
{
}

template <ComputeStage compute_stage>
ADReal
MaterialPropertyUserObjectReaction<compute_stage>::precomputeQpResidual()
{
  ADReal factor = _uo.getData(_current_elem, _qp);
  return factor * _u[_qp];
}
