#include "MovingNodeSetUO.h"
#include "libmesh/id_types.h"

registerMooseObject("raccoonApp", MovingNodeSetUO);

InputParameters
MovingNodeSetUO::validParams()
{
  InputParameters params = NodalUserObject::validParams();
  params.addRequiredCoupledVar("indicator", "The indicator that determines if you apply BC.");
  params.addRequiredParam<Real>("threshold", "The threshold for an element to be in the nodeset.");
  params.addParam<BoundaryName>(
      "moving_boundary_name",
      "Boundary to modify when an element is moved. A boundary with the provided name will be "
      "created if not already exists on the mesh.");
  params.addClassDescription("");
  return params;
}

MovingNodeSetUO::MovingNodeSetUO(const InputParameters & parameters)
  : NodalUserObject(parameters),
    _indicator(coupledValue("indicator")),
    _threshold(getParam<Real>("threshold")),
    _moving_boundary_specified(isParamValid("moving_boundary_name"))
{
}
void
MovingNodeSetUO::initialSetup()
{
  if (_moving_boundary_specified)
  {
    _moving_boundary_name = getParam<BoundaryName>("moving_boundary_name");
    setMovingBoundaryName(_mesh);
  }
}

void
MovingNodeSetUO::setMovingBoundaryName(MooseMesh & mesh)
{
  // We only need one boundary to modify. Create a dummy vector just to use the API.
  const std::vector<BoundaryID> boundary_ids = mesh.getBoundaryIDs({{_moving_boundary_name}}, true);
  mooseAssert(boundary_ids.size() == 1, "Expect exactly one boundary ID.");
  _moving_boundary_id = boundary_ids[0];
  mesh.setBoundaryName(_moving_boundary_id, _moving_boundary_name);
  mesh.getMesh().get_boundary_info().sideset_name(_moving_boundary_id) = _moving_boundary_name;
  mesh.getMesh().get_boundary_info().nodeset_name(_moving_boundary_id) = _moving_boundary_name;
}

void
MovingNodeSetUO::meshChanged()
{
}

void
MovingNodeSetUO::initialize()
{
  _nodes_added.clear();
  _nodes_removed.clear();
}

void
MovingNodeSetUO::execute()
{
  BoundaryInfo & bnd_info = _mesh.getMesh().get_boundary_info();

  // Check if current node is part of the moving boundary
  if (bnd_info.has_boundary_id(_current_node, _moving_boundary_id))
  {
    // if the current node is already part of the boundary check that the
    // indicator still allows it to be in it
    if (_indicator[_qp] < _threshold)
      // Remove this node because it should be in the boundary anymore
      _nodes_removed.insert(_current_node->id());
  }
  else
  {
    // check if a node not in the moving boundary now needs to go into it
    if (_indicator[_qp] >= _threshold)
      _nodes_added.insert(_current_node->id());
  }
}

void
MovingNodeSetUO::finalize()
{
  for (auto node_id : _nodes_removed)
    _mesh.getMesh().get_boundary_info().remove_node(_mesh.nodePtr(node_id), _moving_boundary_id);
  for (auto node_id : _nodes_added)
    _mesh.getMesh().get_boundary_info().add_node(_mesh.nodePtr(node_id), _moving_boundary_id);

  _mesh.getMesh().get_boundary_info().parallel_sync_node_ids();
  _mesh.update();

  // Reinit equation systems
  _fe_problem.meshChanged();
}
