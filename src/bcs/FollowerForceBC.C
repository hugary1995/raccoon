//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "FollowerForceBC.h"
#include "Function.h"
#include "RankTwoTensor.h"

registerADMooseObject("raccoonApp", FollowerForceBC);

InputParameters
FollowerForceBC::validParams()
{
  InputParameters params = ADIntegratedBC::validParams();
  params.addClassDescription("Imposes the pressure boundary condition.");
  params.addParam<FunctionName>("function_x", 0, "The x-direction forcing function");
  params.addParam<FunctionName>("function_y", 0, "The y-direction forcing function");
  params.addParam<FunctionName>("function_z", 0, "The z-direction forcing function");
  params.addParam<std::string>("base_name", "base name for material properties");
  params.addRequiredParam<unsigned int>("component", "component of the displacement variable");
  return params;
}

FollowerForceBC::FollowerForceBC(const InputParameters & parameters)
  : ADIntegratedBC(parameters),
    _function_x(getFunction("function_x")),
    _function_y(getFunction("function_y")),
    _function_z(getFunction("function_z")),
    _component(getParam<unsigned int>("component")),
    _base_name(this->isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _F(getADMaterialProperty<RankTwoTensor>(_base_name + "deformation_gradient"))
{
}

ADReal
FollowerForceBC::computeQpResidual()
{
  RealVectorValue f(_function_x.value(_t, _q_point[_qp]),
                    _function_y.value(_t, _q_point[_qp]),
                    _function_z.value(_t, _q_point[_qp]));
  ADReal f_displaced = _F[_qp].row(_component) * f;
  return -_test[_i][_qp] * f_displaced;
}
