//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADPorousFlowTimeDerivative.h"
#include "MooseVariable.h"
#include "libmesh/quadrature.h"
#include <limits>

registerMooseObject("raccoonApp", ADPorousFlowTimeDerivative);

InputParameters
ADPorousFlowTimeDerivative::validParams()
{
  InputParameters params = ADTimeKernel::validParams();
  params.addClassDescription("Kernel computing fluid content time derivative for multiphase flow.");
  params.addParam<MaterialPropertyName>(
      "porosity_name", "porosity", "Name of porosity material property");
  params.addRequiredCoupledVar("saturation_variable", "fluid phase saturation variable");
  return params;
}

ADPorousFlowTimeDerivative::ADPorousFlowTimeDerivative(const InputParameters & parameters)
  : ADTimeKernel(parameters),
    _porosity(getADMaterialProperty<Real>(getParam<MaterialPropertyName>("porosity_name"))),
    _porosity_old(getMaterialPropertyOld<Real>(getParam<MaterialPropertyName>("porosity_name"))),
    _S(adCoupledValue("saturation_variable")),
    _S_dot(adCoupledDot("saturation_variable"))
{
}

ADReal
ADPorousFlowTimeDerivative::computeQpResidual()
{
  return _test[_i][_qp] *
         (((_porosity[_qp] - _porosity_old[_qp]) / _dt) * _S[_qp] + _porosity[_qp] * _S_dot[_qp]);
}
