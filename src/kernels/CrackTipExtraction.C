//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CrackTipExtraction.h"
#include "Assembly.h"

registerMooseObject("raccoonApp", CrackTipExtraction);

InputParameters
CrackTipExtraction::validParams()
{
  InputParameters params = ADKernel::validParams();
  params += BaseNameInterface::validParams();

  params.addRequiredCoupledVar("phase_field", "The phase-field variable");
  params.addRequiredCoupledVar("levelset", "The levelset variable");
  params.addRequiredParam<Real>("alpha", "penalty");
  params.addRequiredParam<unsigned int>("component", "component of the gradient");
  return params;
}

CrackTipExtraction::CrackTipExtraction(const InputParameters & parameters)
  : ADKernel(parameters),
    BaseNameInterface(parameters),
    _grad_phi(adCoupledGradient("levelset")),
    _d(adCoupledValue("phase_field")),
    _grad_d(adCoupledGradient("phase_field")),
    _alpha(getParam<Real>("alpha")),
    _component(getParam<unsigned int>("component"))
{
}

ADReal
CrackTipExtraction::computeQpResidual()
{
  // ADReal value = _test[_i][_qp] * (_u_nodal[_i] - _wn[_qp]);
  ADReal value = _test[_i][_qp] * (_u[_qp] - _grad_d[_qp](_component));

  if (_d[_qp] > 1e-6)
  {
    ADRealVectorValue n(0, 1, 0);
    if (_grad_d[_qp].norm() > 1e-6)
      n = _grad_d[_qp] / _grad_d[_qp].norm();
    // if (_grad_phi[_qp].norm() > 1e-6)
    //   n = _grad_phi[_qp] / _grad_phi[_qp].norm();
    // ADRealVectorValue n(0, 1, 0);
    value += _alpha * (_grad_test[_i][_qp] * n) * (_grad_u[_qp] * n);
  }

  return value;
}
