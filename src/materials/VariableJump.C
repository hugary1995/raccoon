//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "VariableJump.h"

registerMooseObject("raccoonApp", VariableJump);

InputParameters
VariableJump::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription("This class computes the crack opening displacement.");
  params.addRequiredCoupledVar("phase_field", "The phase-field variable");
  params.addRequiredCoupledVar("level_set", "The levelset variable");
  params.addRequiredCoupledVar("variable", "The jump in this variable");
  params.addRequiredParam<MaterialPropertyName>("jump", "Name of the jump");
  return params;
}

VariableJump::VariableJump(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _wn(declareADProperty<RealVectorValue>(prependBaseName("jump", true))),
    _u(adCoupledValue("variable")),
    _d(adCoupledValue("phase_field")),
    _grad_d(adCoupledGradient("phase_field")),
    _grad_phi(adCoupledGradient("level_set"))
{
}

void
VariableJump::computeQpProperties()
{
  if (_grad_d[_qp].norm() > 1e-12 && _grad_phi[_qp].norm() > 1e-12)
  {
    ADRealVectorValue n = _grad_phi[_qp] / _grad_phi[_qp].norm();
    ADReal grad_d_norm = std::sqrt(_grad_d[_qp] * _grad_d[_qp]);
    _wn[_qp] = -_u[_qp] * n * grad_d_norm;
  }
  else
    _wn[_qp].zero();
}
