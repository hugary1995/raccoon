//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADSPFPressureTimeDerivative.h"

registerMooseObject("raccoonApp", ADSPFPressureTimeDerivative);

InputParameters
ADSPFPressureTimeDerivative::validParams()
{
  InputParameters params = ADTimeDerivative::validParams();
  params.addClassDescription(
      "AD Time derivative of pressure divided by the biot modulus in single phase flow.");
  params.addParam<MaterialPropertyName>("biot_modulus", "biot_modulus", "biot_modulus");
  return params;
}

ADSPFPressureTimeDerivative::ADSPFPressureTimeDerivative(const InputParameters & parameters)
  : ADTimeDerivative(parameters), _M(getMaterialProperty<Real>("biot_modulus"))
{
}

ADReal
ADSPFPressureTimeDerivative::precomputeQpResidual()
{
  return 1 / _M[_qp] * ADTimeDerivative::precomputeQpResidual();
}
