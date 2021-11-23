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

  params.addRequiredCoupledVar("level_set", "The levelset variable");
  params.addRequiredParam<MaterialPropertyName>("alpha", "penalty");
  return params;
}

Nitsche::Nitsche(const InputParameters & parameters)
  : ADKernel(parameters),
    BaseNameInterface(parameters),
    _grad_phi(adCoupledGradient("level_set")),
    _wn(getADMaterialPropertyByName<Real>(prependBaseName("crack_opening_displacement"))),
    _alpha(getADMaterialPropertyByName<Real>(prependBaseName("alpha", true)))
{
}

ADReal
Nitsche::computeQpResidual()
{
  ADReal value = _test[_i][_qp] * (_u[_qp] - _wn[_qp]);

  ADRealVectorValue n = _grad_phi[_qp] / _grad_phi[_qp].norm();
  value += _alpha[_qp] * (_grad_test[_i][_qp] * n) * (_grad_u[_qp] * n);

  return value;
}
