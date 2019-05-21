//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "FollowerForceBC.h"
#include "Function.h"
#include "RankTwoTensor.h"

registerADMooseObject("raccoonApp", FollowerForceBC);

defineADValidParams(
    FollowerForceBC,
    ADIntegratedBC,
    params.addClassDescription("Imposes the pressure boundary condition.");
    params.addParam<FunctionName>("function_x", 0, "The x-direction forcing function");
    params.addParam<FunctionName>("function_y", 0, "The y-direction forcing function");
    params.addParam<FunctionName>("function_z", 0, "The z-direction forcing function");
    params.addParam<std::string>("base_name", "base name for material properties");
    params.addRequiredParam<unsigned int>("component", "component of the displacement variable"););

template <ComputeStage compute_stage>
FollowerForceBC<compute_stage>::FollowerForceBC(const InputParameters & parameters)
  : ADIntegratedBC<compute_stage>(parameters),
    _function_x(getFunction("function_x")),
    _function_y(getFunction("function_y")),
    _function_z(getFunction("function_z")),
    _component(adGetParam<unsigned int>("component")),
    _base_name(this->isParamValid("base_name") ? adGetParam<std::string>("base_name") + "_" : ""),
    _F(adGetADMaterialProperty<RankTwoTensor>(_base_name + "deformation_gradient"))
{
}

template <ComputeStage compute_stage>
ADReal
FollowerForceBC<compute_stage>::computeQpResidual()
{
  RealVectorValue f(_function_x.value(_t, _q_point[_qp]),
                    _function_y.value(_t, _q_point[_qp]),
                    _function_z.value(_t, _q_point[_qp]));
  ADReal f_displaced = _F[_qp].row(_component) * f;
  return -_test[_i][_qp] * f_displaced;
}
