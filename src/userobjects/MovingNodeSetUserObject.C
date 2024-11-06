//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "MovingNodeSetUserObject.h"

registerMooseObject("raccoonApp", MovingNodeSetUserObject);

InputParameters
MovingNodeSetUserObject::validParams()
{
  InputParameters params = NodalUserObject::validParams();
  params.addRequiredCoupledVar("indicator", "The indicator that determines if you apply BC.");
  params.addRequiredParam<Real>("threshold", "The threshold for an element to be in the nodeset.");
  params.addParam<BoundaryName>(
      "moving_boundary_name",
      "Boundary to modify when an element is moved. A boundary with the provided name will be "
      "created if not already exists on the mesh.");
  params.addParam<MooseEnum>("criterion_type",
                             MooseEnum("BELOW EQUAL ABOVE", "ABOVE"),
                             "Criterion to use for the threshold");
  params.addClassDescription("This User Object provides the abilty for a node set to follow a "
                             "variable. Any node that is above the threshold will be moved into "
                             "the nodeset and any node under the threshold will be taken out.");
  return params;
}

MovingNodeSetUserObject::MovingNodeSetUserObject(const InputParameters & parameters)
  : NodalUserObject(parameters),
    _indicator(coupledValue("indicator")),
    _threshold(getParam<Real>("threshold")),
    _moving_boundary_specified(isParamValid("moving_boundary_name")),
    _criterion_type(getParam<MooseEnum>("criterion_type").getEnum<CriterionType>())
{
}
void
MovingNodeSetUserObject::initialSetup()
{
  if (_moving_boundary_specified)
  {
    _moving_boundary_name = getParam<BoundaryName>("moving_boundary_name");
    setMovingBoundaryName(_mesh);
  }
}

void
MovingNodeSetUserObject::setMovingBoundaryName(MooseMesh & mesh)
{
  // We only need one boundary to modify. Create a dummy vector just to use the API.
  const std::vector<BoundaryID> boundary_ids = mesh.getBoundaryIDs({{_moving_boundary_name}}, true);
  mooseAssert(boundary_ids.size() == 1, "Expect exactly one boundary ID.");
  _moving_boundary_id = boundary_ids[0];
  mesh.setBoundaryName(_moving_boundary_id, _moving_boundary_name);
  mesh.getMesh().get_boundary_info().nodeset_name(_moving_boundary_id) = _moving_boundary_name;
}

void
MovingNodeSetUserObject::initialize()
{
  _nodes_added.clear();
  _nodes_removed.clear();
}

void
MovingNodeSetUserObject::execute()
{

  BoundaryInfo & bnd_info = _mesh.getMesh().get_boundary_info();

  // Check if current node is part of the moving boundary
  if (bnd_info.has_boundary_id(_current_node, _moving_boundary_id))
  {
    // If the current node is already part of the boundary check that the
    // meets the criteria to stay in it
    if (!computeCriterion())
      // Remove this node because it should be in the boundary anymore
      _nodes_removed.push_back(_current_node->id());
  }
  else
  {
    // check if a node not in the moving boundary now needs to go into it
    if (computeCriterion())
      _nodes_added.push_back(_current_node->id());
  }
}

void
MovingNodeSetUserObject::finalize()
{
  BoundaryInfo & bnd_info = _mesh.getMesh().get_boundary_info();

  for (auto node_id : _nodes_removed)
    bnd_info.remove_node(_mesh.nodePtr(node_id), _moving_boundary_id);

  for (auto node_id : _nodes_added)
    bnd_info.add_node(_mesh.nodePtr(node_id), _moving_boundary_id);

  bnd_info.parallel_sync_node_ids();
  _mesh.update();

  // Reinit equation systems
  _fe_problem.meshChanged();
}

Real
MovingNodeSetUserObject::computeCriterion()
{

  Real criterion = _indicator[_qp];

  bool criterion_met = false;
  switch (_criterion_type)
  {
    case CriterionType::Equal:
      criterion_met = MooseUtils::absoluteFuzzyEqual(criterion, _threshold);
      break;

    case CriterionType::Below:
      criterion_met = criterion < _threshold;
      break;

    case CriterionType::Above:
      criterion_met = criterion > _threshold;
      break;
  }

  return criterion_met;
}
