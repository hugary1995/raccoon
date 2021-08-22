//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CODDescent.h"
#include "Assembly.h"

registerMooseObject("raccoonApp", CODDescent);

InputParameters
CODDescent::validParams()
{
  InputParameters params = ADKernel::validParams();
  params += BaseNameInterface::validParams();

  params.addRequiredCoupledVar("cod_density", "The crack opening density");
  params.addRequiredCoupledVar("phase_field", "The phase field");
  return params;
}

CODDescent::CODDescent(const InputParameters & parameters)
  : ADKernel(parameters),
    BaseNameInterface(parameters),
    _grad_d(adCoupledGradient("phase_field")),
    _grad_w(adCoupledGradient("cod_density"))
{
}

ADReal
CODDescent::computeQpResidual()
{
  ADRealVectorValue n;
  if (_grad_d[_qp].norm() > 1e-6)
    n = _grad_d[_qp] / _grad_d[_qp].norm();

  ADReal value = _test[_i][_qp] * (_u[_qp] - _grad_w[_qp] * n);

  return value;
}
