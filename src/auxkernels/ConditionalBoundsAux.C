#include "ConditionalBoundsAux.h"

registerMooseObject("raccoonApp", ConditionalBoundsAux);

InputParameters
ConditionalBoundsAux::validParams()
{
  InputParameters params = BoundsBase::validParams();
  params.addClassDescription(
      "This class conditionally enforces a lower bound. When the variable value is below a given "
      "threshold, a constant value is used as the bound; when the variable value is above a given "
      "threshold, irreversibility is enforced.");
  params.addRequiredParam<Real>("fixed_bound_value", "The value of fixed bound for the variable");
  params.addRequiredParam<Real>("threshold_value",
                                "The threshold for conditional history bound for the variable");
  params.set<MooseEnum>("bound_type") = "lower";
  params.suppressParameter<MooseEnum>("bound_type");
  return params;
}

ConditionalBoundsAux::ConditionalBoundsAux(const InputParameters & parameters)
  : BoundsBase(parameters),
    _fixed_bound_value(getParam<Real>("fixed_bound_value")),
    _threshold_value(getParam<Real>("threshold_value"))
{
}

Real
ConditionalBoundsAux::getBound()
{
  Real d_old = 0;
  if (_fe_var && isNodal())
    d_old = _fe_var->getNodalValueOld(*_current_node);
  else
    mooseError("This variable type is not supported yet");
  if (d_old >= _threshold_value)
    return d_old;
  else
    return _fixed_bound_value;
}
