//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CoupledNeumannBC.h"

registerMooseObject("raccoonApp", CoupledNeumannBC);

InputParameters
CoupledNeumannBC::validParams()
{
  InputParameters params = IntegratedBC::validParams();
  params.addClassDescription(
      "computes the weak form $\\int_\\bodyboundary - w c v \\dArea$, where $w$ is the test "
      "function, $c$ is a coefficient, $v$ is a coupled variable.");
  params.addParam<Real>("coef", 1.0, "Value multiplied by the coupled value on the boundary");
  params.addRequiredCoupledVar("coupled_var", "Flux value at the boundary");
  return params;
}

CoupledNeumannBC::CoupledNeumannBC(const InputParameters & parameters)
  : IntegratedBC(parameters), _coef(getParam<Real>("coef")), _v(coupledValue("coupled_var"))
{
}

Real
CoupledNeumannBC::computeQpResidual()
{
  return -_test[_i][_qp] * _coef * _v[_qp];
}
