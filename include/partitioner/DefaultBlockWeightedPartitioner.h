//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "BlockWeightedPartitioner.h"

/**
 * A BlockWeightedPartitioner that does not require every block to be given a weight. Elements in
 * blocks that are not listed get a default weight. This is needed when blocks are added to the
 * mesh after the first partition, e.g. the lower-dimensional blocks created by the mortar contact
 * action, since those blocks cannot be listed in 'block'.
 */
class DefaultBlockWeightedPartitioner : public BlockWeightedPartitioner
{
public:
  static InputParameters validParams();

  DefaultBlockWeightedPartitioner(const InputParameters & params);

  /**
   * MOOSE made computeElementWeight const. Both signatures are declared, without override, so that
   * whichever one the MOOSE in use declares virtual is the one that gets overridden.
   */
  ///@{
  virtual dof_id_type computeElementWeight(Elem & elem);
  virtual dof_id_type computeElementWeight(Elem & elem) const;
  ///@}

  void initialize(MeshBase & mesh) override;

private:
  /// The weight of an element, falling back to the default weight for unlisted blocks
  dof_id_type elementWeight(const Elem & elem) const;

  /// The weight given to elements in blocks that are not listed in 'block'
  const dof_id_type _default_weight;

  /// A map from subdomain to weight for the listed blocks
  std::unordered_map<SubdomainID, dof_id_type> _listed_blocks_to_weights;
};
