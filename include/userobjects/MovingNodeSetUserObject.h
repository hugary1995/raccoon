//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "NodalUserObject.h"

/**
 */
class MovingNodeSetUserObject : public NodalUserObject
{
public:
  static InputParameters validParams();

  MovingNodeSetUserObject(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual void finalize() override;
  virtual void threadJoin(const UserObject & /* uo*/) override{};
  virtual void initialSetup() override;

protected:
  virtual Real computeCriterion();

private:
  // Set the name of the moving boundary. Create the nodeset/sideset if not exist.
  void setMovingBoundaryName(MooseMesh & mesh);

  /// Indicator that determines when a nodeset moves
  const VariableValue & _indicator;
  /// Threshold to modify the node
  const Real _threshold;

  /// Whether a moving boundary name is provided
  const bool _moving_boundary_specified;
  /// The name of the moving boundary
  BoundaryName _moving_boundary_name;

  /// The id of the moving boundary
  BoundaryID _moving_boundary_id;

  /// Nodes that need to be added to the nodeset
  std::vector<dof_id_type> _nodes_added;
  /// Nodes that needs to be deleted from the nodeset;
  std::vector<dof_id_type> _nodes_removed;

  /// Criterion type
  const enum class CriterionType { Below, Equal, Above } _criterion_type;
};
