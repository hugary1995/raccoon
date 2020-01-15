//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PressureBC.h"
#include "Function.h"

registerADMooseObject("raccoonApp", PressureBC);

defineADLegacyParams(PressureBC);

template <ComputeStage compute_stage>
InputParameters
PressureBC<compute_stage>::validParams()
{
  InputParameters params = ADIntegratedBC<compute_stage>::validParams();
  params.addClassDescription("Imposes the pressure boundary condition.");
  params.addRequiredParam<FunctionName>("function",
                                        "The function describing the pressure magnitude");
  params.set<bool>("use_displaced_mesh") = true;
  params.addRequiredParam<unsigned int>("component", "component of pressure");
  return params;
}

template <ComputeStage compute_stage>
PressureBC<compute_stage>::PressureBC(const InputParameters & parameters)
  : ADIntegratedBC<compute_stage>(parameters),
    _function(getFunction("function")),
    _component(getParam<unsigned int>("component"))
{
}

template <ComputeStage compute_stage>
ADReal
PressureBC<compute_stage>::computeQpResidual()
{
  return _test[_i][_qp] * _function.value(_t, _q_point[_qp]) * _normals[_qp](_component);
}
