//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADPorousFlowTimeDerivative.h"

registerMooseObject("raccoonApp", ADPorousFlowTimeDerivative);

InputParameters
ADPorousFlowTimeDerivative::validParams()
{
  InputParameters params = ADTimeKernel::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription("Kernel computing fluid content time derivative for multiphase flow.");
  params.addParam<MaterialPropertyName>(
      "porosity", "porosity", "Name of the porosity material property");
  params.addRequiredCoupledVar("saturation", "Fluid phase saturation variable");
  return params;
}

ADPorousFlowTimeDerivative::ADPorousFlowTimeDerivative(const InputParameters & parameters)
  : ADTimeKernel(parameters),
    BaseNameInterface(parameters),
    _porosity(getADMaterialProperty<Real>(prependBaseName("porosity", true))),
    _porosity_old(getMaterialPropertyOld<Real>(prependBaseName("porosity", true))),
    _S(adCoupledValue("saturation")),
    _S_dot(adCoupledDot("saturation"))
{
}

ADReal
ADPorousFlowTimeDerivative::computeQpResidual()
{
  ADReal porosity_dot = (_porosity[_qp] - _porosity_old[_qp]) / _dt;
  return _test[_i][_qp] * (porosity_dot * _S[_qp] + _porosity[_qp] * _S_dot[_qp]);
}
