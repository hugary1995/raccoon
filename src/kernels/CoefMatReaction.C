//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CoefMatReaction.h"

registerADMooseObject("raccoonApp", CoefMatReaction);

InputParameters
CoefMatReaction::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  params.addClassDescription(
      "Reaction term optionally multiplied with a coefficient and a material property");
  params.addParam<Real>("coefficient", 1.0, "Coefficient of the term");
  params.addParam<MaterialPropertyName>("material_property_name",
                                        "name of the material property to provide the multiplier");
  return params;
}

CoefMatReaction::CoefMatReaction(const InputParameters & parameters)
  : ADKernelValue(parameters),
    _coef(getParam<Real>("coefficient")),
    _prop(isParamValid("material_property_name")
              ? &getMaterialProperty<Real>("material_property_name")
              : NULL)
{
}

ADReal
CoefMatReaction::precomputeQpResidual()
{
  ADReal factor = _coef * (_prop ? (*_prop)[_qp] : 1.0);
  return factor * _u[_qp];
}
