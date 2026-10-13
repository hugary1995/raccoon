//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "DefaultBlockWeightedPartitioner.h"

#include "MooseMeshUtils.h"

#include "libmesh/elem.h"

registerMooseObject("raccoonApp", DefaultBlockWeightedPartitioner);

InputParameters
DefaultBlockWeightedPartitioner::validParams()
{
  InputParameters params = BlockWeightedPartitioner::validParams();

  params.addParam<dof_id_type>(
      "default_weight",
      0,
      "The weight (integer) given to elements in blocks that are not listed in 'block'");

  params.addClassDescription("Partition mesh by weighting blocks, with a default weight for the "
                             "blocks that are not given one");

  return params;
}

DefaultBlockWeightedPartitioner::DefaultBlockWeightedPartitioner(const InputParameters & params)
  : BlockWeightedPartitioner(params), _default_weight(getParam<dof_id_type>("default_weight"))
{
}

void
DefaultBlockWeightedPartitioner::initialize(MeshBase & mesh)
{
  // Checks that the listed blocks exist on the mesh
  BlockWeightedPartitioner::initialize(mesh);

  const auto block_ids =
      MooseMeshUtils::getSubdomainIDs(mesh, getParam<std::vector<SubdomainName>>("block"));
  const auto & weights = getParam<std::vector<dof_id_type>>("weight");

  _listed_blocks_to_weights.clear();
  for (const auto i : index_range(block_ids))
    _listed_blocks_to_weights[block_ids[i]] = weights[i];
}

dof_id_type
DefaultBlockWeightedPartitioner::computeElementWeight(Elem & elem)
{
  return elementWeight(elem);
}

dof_id_type
DefaultBlockWeightedPartitioner::computeElementWeight(Elem & elem) const
{
  return elementWeight(elem);
}

dof_id_type
DefaultBlockWeightedPartitioner::elementWeight(const Elem & elem) const
{
  const auto it = _listed_blocks_to_weights.find(elem.subdomain_id());
  return it != _listed_blocks_to_weights.end() ? it->second : _default_weight;
}
