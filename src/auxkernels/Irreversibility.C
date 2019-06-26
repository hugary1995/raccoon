//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "Irreversibility.h"
#include "MooseVariable.h"
#include "Executioner.h"
#include "SystemBase.h"

registerMooseObject("raccoonApp", Irreversibility);

template <>
InputParameters
validParams<Irreversibility>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("bounded_var", "variable to be bounded");
  params.addRequiredParam<VariableName>("lower", "lower bound");
  params.addParam<Real>("upper", 1.0, "upper bound");
  return params;
}

Irreversibility::Irreversibility(const InputParameters & parameters)
  : AuxKernel(parameters),
    _upper_vector(_nl_sys.getVector("upper_bound")),
    _lower_vector(_nl_sys.getVector("lower_bound")),
    _bounded_var_num(coupled("bounded_var")),
    _d_var(_subproblem.getStandardVariable(_tid, getParam<VariableName>("lower"))),
    _upper_bound(getParam<Real>("upper"))
{
  if (!isNodal())
    mooseError("Irreversibility must be used on a nodal auxiliary variable!");

  const std::vector<std::string> & solver_options =
      _app.getExecutioner()->getParam<std::vector<std::string>>("petsc_options_value");
  if (std::find(solver_options.begin(), solver_options.end(), "vinewtonrsls") ==
          solver_options.end() &&
      std::find(solver_options.begin(), solver_options.end(), "vinewtonssls") ==
          solver_options.end())
    mooseError("A variational solver, i.e. vinewtonrsls, must be used to solve NCP");
}

Real
Irreversibility::computeValue()
{
  if (_current_node->n_dofs(_nl_sys.number(), _bounded_var_num) > 0)
  {
    Real lower_bound = _d_var.getNodalValueOld(*_current_node);
    // The zero is for the component, this will only work for Lagrange variables!
    dof_id_type dof = _current_node->dof_number(_nl_sys.number(), _bounded_var_num, 0);
    _upper_vector.set(dof, _upper_bound);
    _lower_vector.set(dof, lower_bound);
  }

  return 0.0;
}
