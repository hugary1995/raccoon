//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ScalarDirichletBC.h"

registerMooseObject("raccoonApp", ScalarDirichletBC);

template <>
InputParameters
validParams<ScalarDirichletBC>()
{
  InputParameters params = validParams<NodalBC>();
  params.addRequiredCoupledVar("scalar_var", "scalar variable");
  params.addClassDescription("Imposes the essential boundary condition $u=s$, where $s$ "
                             "is a scalar");
  return params;
}

ScalarDirichletBC::ScalarDirichletBC(const InputParameters & parameters)
  : NodalBC(parameters), _scalar_val(coupledScalarValue("scalar_var"))
{
}

Real
ScalarDirichletBC::computeQpResidual()
{
  return _u[_qp] - _scalar_val[0];
}
