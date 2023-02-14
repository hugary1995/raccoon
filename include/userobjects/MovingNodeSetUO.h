#pragma once

#include "MooseTypes.h"
#include "NodalUserObject.h"
#include "NonlinearSystemBase.h"
#include "AuxiliarySystem.h"
#include "libmesh/id_types.h"
#include <set>
/**
 */
class MovingNodeSetUO : public NodalUserObject
{
public:
  static InputParameters validParams();

  MovingNodeSetUO(const InputParameters & parameters);

  virtual void meshChanged() override;
  virtual void initialize() override;
  virtual void execute() override;
  virtual void finalize() override;
  virtual void threadJoin(const UserObject & /* uo*/) override{};
  virtual void initialSetup() override;

private:
  // Set the name of the moving boundary. Create the nodeset/sideset if not exist.
  void setMovingBoundaryName(MooseMesh & mesh);

  /// Indicator that determines when a nodeset moves
  const VariableValue & _indicator;
  const Real _threshold;

  /// Whether a moving boundary name is provided
  const bool _moving_boundary_specified;
  /// The name of the moving boundary
  BoundaryName _moving_boundary_name;

  /// The id of the moving boundary
  BoundaryID _moving_boundary_id;

  /// Nodes that need to be added to the nodeset
  std::set<dof_id_type> _nodes_added;
  /// Nodes that needs to be deleted from the nodeset;
  std::set<dof_id_type> _nodes_removed;
};
