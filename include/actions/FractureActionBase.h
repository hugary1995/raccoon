#ifndef FractureActionBase_H
#define FractureActionBase_H

// MOOSE includes
#include "Action.h"
#include "AddVariableAction.h"
// LIBMESH includes
#include "libmesh/fe_type.h"

// Forward declaration
class FractureActionBase;

template <>
InputParameters validParams<FractureActionBase>();

class FractureActionBase : public Action
{
public:
  FractureActionBase(const InputParameters & params);

  virtual void act();

protected:
  /**
   * Get the block ids from the input parameters
   * @return A set of block ids defined in the input
   */
  std::set<SubdomainID> getSubdomainIDs();
  /// Name of the variable being created
  const NonlinearVariableName _var_name;
  /// FEType for the variable being created
  const FEType _fe_type;
  /// nonlinear displacement variables
  const std::vector<VariableName> _displacements;
};

#endif // NFractureActionBase_H
