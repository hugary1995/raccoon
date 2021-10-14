//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADSPFPressureTimeDerivative.h"

registerMooseObject("raccoonApp", ADSPFPressureTimeDerivative);

InputParameters
ADSPFPressureTimeDerivative::validParams()
{
  InputParameters params = ADTimeDerivative::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription(
      "The time derivative of pressure normalized by the Biot modulus in single phase flow.");
  params.addParam<MaterialPropertyName>("biot_modulus", "biot_modulus", "The Biot modulus");
  return params;
}

ADSPFPressureTimeDerivative::ADSPFPressureTimeDerivative(const InputParameters & parameters)
  : ADTimeDerivative(parameters),
    BaseNameInterface(parameters),
    _M(getADMaterialProperty<Real>(prependBaseName("biot_modulus", true)))
{
}

ADReal
ADSPFPressureTimeDerivative::precomputeQpResidual()
{
  return 1 / _M[_qp] * ADTimeDerivative::precomputeQpResidual();
}
