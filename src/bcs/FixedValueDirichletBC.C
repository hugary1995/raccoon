//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "FixedValueDirichletBC.h"

registerMooseObject("raccoonApp", FixedValueDirichletBC);

InputParameters
FixedValueDirichletBC::validParams()
{
  InputParameters params = DirichletBCBase::validParams();
  params.addClassDescription("Do not change the variable value over the timestep by prescribing "
                             "its old value as a Dirichlet BC. This object only makes sense when "
                             "used in conjunction with the Control system.");
  return params;
}

FixedValueDirichletBC::FixedValueDirichletBC(const InputParameters & parameters)
  : DirichletBCBase(parameters), _u_old(_var.dofValuesOld())
{
}

Real
FixedValueDirichletBC::computeQpValue()
{
  return _u_old[_qp];
}
