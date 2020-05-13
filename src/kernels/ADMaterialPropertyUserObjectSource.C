//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADMaterialPropertyUserObjectSource.h"

registerADMooseObject("raccoonApp", ADMaterialPropertyUserObjectSource);

InputParameters
ADMaterialPropertyUserObjectSource::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  params.addClassDescription(
      "Source term optionally multiplied with a material property stored in a user object");
  params.addRequiredParam<UserObjectName>("uo_name", "userobject that has values at qps");
  params.addParam<Real>("coef", 1.0, "coefficient of the source term");
  return params;
}

ADMaterialPropertyUserObjectSource::ADMaterialPropertyUserObjectSource(
    const InputParameters & parameters)
  : ADKernelValue(parameters),
    _uo(getUserObject<ADMaterialPropertyUserObject>("uo_name")),
    _coef(getParam<Real>("coef"))
{
}

ADReal
ADMaterialPropertyUserObjectSource::precomputeQpResidual()
{
  return _coef * _uo.getData(_current_elem, _qp);
}
