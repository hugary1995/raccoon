//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADMaterialPropertyUserObjectDiffusion.h"
#include "Assembly.h"

registerADMooseObject("raccoonApp", ADMaterialPropertyUserObjectDiffusion);

InputParameters
ADMaterialPropertyUserObjectDiffusion::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addClassDescription(
      "Diffusion term optionally multiplied with a material property stored in a user object");
  params.addRequiredParam<UserObjectName>("uo_name", "userobject that has values at qps");
  params.addParam<Real>("coef", 1.0, "coefficient of the source term");
  return params;
}

ADMaterialPropertyUserObjectDiffusion::ADMaterialPropertyUserObjectDiffusion(
    const InputParameters & parameters)
  : ADKernel(parameters),
    _uo(getUserObject<ADMaterialPropertyUserObject>("uo_name")),
    _coef(getParam<Real>("coef")),
    _coord_sys(_assembly.coordSystem())
{
}

ADReal
ADMaterialPropertyUserObjectDiffusion::computeQpResidual()
{
  ADReal factor = _coef * _uo.getData(_current_elem, _qp);

  ADReal value = _grad_test[_i][_qp] * _grad_u[_qp];
  if (_coord_sys == Moose::COORD_RZ)
    value -= _test[_i][_qp] / _ad_q_point[_qp](0) * _grad_u[_qp](0);

  return factor * value;
}
