#ifndef DamageCouplingAction_H
#define DamageCouplingAction_H

// MOOSE includes
#include "Action.h"
#include "AddVariableAction.h"
// LIBMESH includes
#include "libmesh/fe_type.h"

// Forward declaration
class DamageCouplingAction;

template <>
InputParameters validParams<DamageCouplingAction>();

class DamageCouplingAction : public Action
{
public:
  DamageCouplingAction(const InputParameters & params);

  virtual void act();

protected:
  /// Name of the variable being created
  const NonlinearVariableName _var_name;
  /// FEType for the variable being created
  const FEType _fe_type;
  /// nonlinear displacement variables
  const std::vector<VariableName> _displacements;
};

#endif // NDamageCouplingAction_H
