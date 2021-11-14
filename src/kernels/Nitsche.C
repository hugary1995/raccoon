//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "Nitsche.h"

registerMooseObject("raccoonApp", Nitsche);

InputParameters
Nitsche::validParams()
{
  InputParameters params = ADKernel::validParams();
  params += BaseNameInterface::validParams();

  params.addRequiredCoupledVar("phase_field", "The phase-field variable");
  params.addRequiredParam<MaterialPropertyName>("alpha", "penalty");
  return params;
}

Nitsche::Nitsche(const InputParameters & parameters)
  : ADKernel(parameters),
    BaseNameInterface(parameters),
    _d(adCoupledValue("phase_field")),
    _grad_d(adCoupledGradient("phase_field")),
    _wn(getADMaterialPropertyByName<Real>(prependBaseName("crack_opening_displacement"))),
    _alpha(getADMaterialPropertyByName<Real>(prependBaseName("alpha", true)))
{
}

ADReal
Nitsche::computeQpResidual()
{
  ADReal value = _test[_i][_qp] * (_u[_qp] - _wn[_qp]);

  ADReal grad_d_norm = std::sqrt(_grad_d[_qp] * _grad_d[_qp] + 1e-12);
  ADRealVectorValue n = _grad_d[_qp] / grad_d_norm;
  value += _alpha[_qp] * (_grad_test[_i][_qp] * n) * (_grad_u[_qp] * n);

  return value;
}
