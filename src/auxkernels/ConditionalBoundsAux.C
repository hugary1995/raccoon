#include "ConditionalBoundsAux.h"

registerMooseObject("raccoonApp", ConditionalBoundsAux);

defineLegacyParams(ConditionalBoundsAux);

InputParameters
ConditionalBoundsAux::validParams()
{
  InputParameters params = BoundsAuxBase::validParams();
  params.addClassDescription(
      "Provides the bound of the variable a lower bound according to variable "
      "value"
      "for kumar's phase-field fracture model 2020"
      "variable to PETSc's SNES variational inequalities solver.");
  params.addRequiredParam<Real>(
      "fixed_bound_value",
      "The value of fixed bound for the variable");
  params.addRequiredParam<Real>(
      "threshold_value",
      "The threshold for conditional history bound for the variable");
  return params;
}

ConditionalBoundsAux::ConditionalBoundsAux(const InputParameters & parameters)
  : BoundsAuxBase(parameters),
    _fixed_bound_value(getParam<Real>("fixed_bound_value")),
    _threshold_value(getParam<Real>("threshold_value"))
{
}

Real
ConditionalBoundsAux::getBound()
{
  Real d_old = _var.getNodalValueOld(*_current_node);
  if (d_old >= _threshold_value)
  {
    return d_old;
  }
  else
  {
    return _fixed_bound_value;
  }
}
