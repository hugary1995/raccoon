//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ComputeFiniteStrain.h"

/**
 * ComputeLowerDimensionalFiniteStrain defines a strain increment and a rotation increment
 * for finite strains in 2D geometries, handling the out of plane strains.
 * ComputeLowerDimensionalFiniteStrain contains a virtual method to define the out-of-plane strain
 * as a general nonzero value in the inherited classes ComputePlaneFiniteStrain
 * and ComputeAxisymmetricRZFiniteStrain.
 */
class ComputeLowerDimensionalFiniteStrain : public ComputeFiniteStrain
{
public:
  static InputParameters validParams();

  ComputeLowerDimensionalFiniteStrain(const InputParameters & parameters);

  virtual void computeProperties() override;

protected:
  virtual void initQpStatefulProperties() override;

  const MaterialProperty<RankTwoTensor> & _Fbar;
  MaterialProperty<RankTwoTensor> & _Q;
  const VariableValue & _out_of_plane_strain;
};
