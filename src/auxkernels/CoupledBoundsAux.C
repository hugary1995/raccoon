//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CoupledBoundsAux.h"

registerMooseObject("raccoonApp", CoupledBoundsAux);

InputParameters
CoupledBoundsAux::validParams()
{
  InputParameters params = BoundsAuxBase::validParams();
  params.addClassDescription("Provides the upper or lower bound using a coupled variable");
  params.addRequiredParam<VariableName>("coupled_variable",
                                        "coupled variable to provide the bound");
  return params;
}

CoupledBoundsAux::CoupledBoundsAux(const InputParameters & parameters)
  : BoundsAuxBase(parameters),
    _coupled_var(_subproblem.getStandardVariable(_tid, getParam<VariableName>("coupled_variable")))
{
}

Real
CoupledBoundsAux::getBound()
{
  return _coupled_var.getNodalValue(*_current_node);
}
