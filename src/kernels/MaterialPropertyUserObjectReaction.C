//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "MaterialPropertyUserObjectReaction.h"

registerADMooseObject("raccoonApp", MaterialPropertyUserObjectReaction);

InputParameters
MaterialPropertyUserObjectReaction::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  params.addClassDescription(
      "Reaction term optionally multiplied with a material property stored in a user object");
  params.addRequiredParam<UserObjectName>("uo_name", "userobject that has values at qps");
  return params;
}

MaterialPropertyUserObjectReaction::MaterialPropertyUserObjectReaction(
    const InputParameters & parameters)
  : ADKernelValue(parameters), _uo(getUserObject<MaterialPropertyUserObject>("uo_name"))
{
}

ADReal
MaterialPropertyUserObjectReaction::precomputeQpResidual()
{
  ADReal factor = _uo.getData(_current_elem, _qp);
  return factor * _u[_qp];
}
