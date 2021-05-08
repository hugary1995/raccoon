//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PressureBC.h"
#include "Function.h"

registerMooseObject("raccoonApp", PressureBC);

InputParameters
PressureBC::validParams()
{
  InputParameters params = ADIntegratedBC::validParams();
  params.addClassDescription("Imposes the pressure boundary condition.");
  params.addRequiredParam<FunctionName>("function",
                                        "The function describing the pressure magnitude");
  params.set<bool>("use_displaced_mesh") = true;
  params.addRequiredParam<unsigned int>("component", "component of pressure");
  return params;
}

PressureBC::PressureBC(const InputParameters & parameters)
  : ADIntegratedBC(parameters),
    _function(getFunction("function")),
    _component(getParam<unsigned int>("component"))
{
}

ADReal
PressureBC::computeQpResidual()
{
  return _test[_i][_qp] * _function.value(_t, _q_point[_qp]) * _normals[_qp](_component);
}
