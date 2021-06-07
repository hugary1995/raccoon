//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LowerDimensionalStressDivergenceTensors.h"

registerMooseObject("raccoonApp", LowerDimensionalStressDivergenceTensors);

InputParameters
LowerDimensionalStressDivergenceTensors::validParams()
{
  InputParameters params = StressDivergenceTensors::validParams();
  params.addClassDescription("Stress divergence kernel for lower dimensional elements");
  params.addParam<MaterialPropertyName>(
      "thickness", "thickness", "Thickness of the lower dimensional block");
  return params;
}

LowerDimensionalStressDivergenceTensors::LowerDimensionalStressDivergenceTensors(
    const InputParameters & parameters)
  : StressDivergenceTensors(parameters),
    _thickness(getMaterialProperty<Real>(_base_name + getParam<MaterialPropertyName>("thickness")))
{
}

Real
LowerDimensionalStressDivergenceTensors::computeQpResidual()
{
  return _thickness[_qp] * StressDivergenceTensors::computeQpResidual();
}

Real
LowerDimensionalStressDivergenceTensors::computeQpJacobian()
{
  return _thickness[_qp] * StressDivergenceTensors::computeQpJacobian();
}

Real
LowerDimensionalStressDivergenceTensors::computeQpOffDiagJacobian(unsigned int jvar)
{
  return _thickness[_qp] * StressDivergenceTensors::computeQpOffDiagJacobian(jvar);
}
