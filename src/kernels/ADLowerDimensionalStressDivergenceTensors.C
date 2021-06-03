//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADLowerDimensionalStressDivergenceTensors.h"

registerMooseObject("raccoonApp", ADLowerDimensionalStressDivergenceTensors);

InputParameters
ADLowerDimensionalStressDivergenceTensors::validParams()
{
  InputParameters params = ADStressDivergenceTensors::validParams();
  params.addClassDescription("Stress divergence kernel for lower dimensional elements");
  params.addParam<MaterialPropertyName>(
      "thickness", "thickness", "Thickness of the lower dimensional block");
  return params;
}

ADLowerDimensionalStressDivergenceTensors::ADLowerDimensionalStressDivergenceTensors(
    const InputParameters & parameters)
  : ADStressDivergenceTensors(parameters),
    _thickness(getMaterialProperty<Real>(_base_name + getParam<MaterialPropertyName>("thickness")))
{
}

ADReal
ADLowerDimensionalStressDivergenceTensors::computeQpResidual()
{
  return _thickness[_qp] * ADStressDivergenceTensors::computeQpResidual();
}
