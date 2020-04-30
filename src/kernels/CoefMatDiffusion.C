//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CoefMatDiffusion.h"

registerADMooseObject("raccoonApp", CoefMatDiffusion);

InputParameters
CoefMatDiffusion::validParams()
{
  InputParameters params = ADKernelGrad::validParams();
  params.addClassDescription(
      "Reaction term optionally multiplied with a coefficient and a material property");
  params.addParam<Real>("coefficient", 1.0, "Coefficient of the term");
  params.addParam<MaterialPropertyName>("material_property_name",
                                        "name of the material property to provide the multiplier");
  return params;
}

CoefMatDiffusion::CoefMatDiffusion(const InputParameters & parameters)
  : ADKernelGrad(parameters),
    _coef(getParam<Real>("coefficient")),
    _prop(isParamValid("material_property_name")
              ? &getMaterialProperty<Real>("material_property_name")
              : NULL)
{
}

ADRealVectorValue
CoefMatDiffusion::precomputeQpResidual()
{
  ADReal factor = _coef * (_prop ? (*_prop)[_qp] : 1.0);
  return factor * _grad_u[_qp];
}
