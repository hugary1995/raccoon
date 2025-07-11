//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "libmesh/ghosting_functor.h"
#include "libmesh/mesh_base.h"

using namespace libMesh;

class AugmentSparsityToDense : public GhostingFunctor
{
private:
  MeshBase & _mesh;

public:
  /**
   * Constructor.
   */
  AugmentSparsityToDense(MeshBase & mesh) : _mesh(mesh) {}

  /**
   * User-defined function to augment the sparsity pattern.
   */
  virtual void operator()(const MeshBase::const_element_iterator & range_begin,
                          const MeshBase::const_element_iterator & range_end,
                          processor_id_type p,
                          map_type & coupled_elements) override;
};
