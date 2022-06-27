//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "JumpEstimator.h"

registerMooseObject("raccoonApp", JumpEstimator);

InputParameters
JumpEstimator::validParams()
{
  InputParameters params = ADKernel::validParams();
  params += BaseNameInterface::validParams();
  params.addRequiredCoupledVar("level_set", "The levelset variable");
  params.addRequiredParam<MaterialPropertyName>("alpha", "penalty");
  params.addRequiredParam<MaterialPropertyName>("jump", "The jump to integrate");
  params.addRequiredParam<unsigned int>("component", "component of the jump to integrate");
  params.addRequiredParam<Real>("support", "support length");
  return params;
}

JumpEstimator::JumpEstimator(const InputParameters & parameters)
  : ADKernel(parameters),
    BaseNameInterface(parameters),
    _grad_phi(adCoupledGradient("level_set")),
    _wn(getADMaterialPropertyByName<RealVectorValue>(prependBaseName("jump", true))),
    _alpha(getADMaterialPropertyByName<Real>(prependBaseName("alpha", true))),
    _component(getParam<unsigned int>("component")),
    _support(getParam<Real>("support"))
{
}

ADReal
JumpEstimator::computeQpResidual()
{
  ADReal value = _test[_i][_qp] * (_u[_qp] - _support * _wn[_qp](_component));

  ADRealVectorValue n = _grad_phi[_qp] / _grad_phi[_qp].norm();
  value += _alpha[_qp] * (_grad_test[_i][_qp] * n) * (_grad_u[_qp] * n);

  return value;
}
