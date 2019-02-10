#ifndef BrittleFractureAction_H
#define BrittleFractureAction_H

// MOOSE includes
#include "Action.h"
#include "AddVariableAction.h"
// LIBMESH includes
#include "libmesh/fe_type.h"

// Forward declaration
class BrittleFractureAction;

template <>
InputParameters validParams<BrittleFractureAction>();

class BrittleFractureAction : public Action
{
public:
  BrittleFractureAction(const InputParameters & params);

  virtual void act();

protected:
  /// Name of the variable being created
  const NonlinearVariableName _var_name;
  /// FEType for the variable being created
  const FEType _fe_type;
  /// nonlinear displacement variables
  const std::vector<VariableName> _displacements;
};

#endif // NBrittleFractureAction_H
