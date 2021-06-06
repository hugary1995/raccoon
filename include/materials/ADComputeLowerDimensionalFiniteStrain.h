//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADComputeFiniteStrain.h"

/**
 * ADComputeLowerDimensionalFiniteStrain defines a strain increment and a rotation increment
 * for finite strains in 2D geometries, handling the out of plane strains.
 * ADComputeLowerDimensionalFiniteStrain contains a virtual method to define the out-of-plane strain
 * as a general nonzero value in the inherited classes ComputePlaneFiniteStrain
 * and ComputeAxisymmetricRZFiniteStrain.
 */
class ADComputeLowerDimensionalFiniteStrain : public ADComputeFiniteStrain
{
public:
  static InputParameters validParams();

  ADComputeLowerDimensionalFiniteStrain(const InputParameters & parameters);

  virtual void computeProperties() override;

protected:
  MaterialProperty<RankTwoTensor> & _Q;
  const ADVariableValue & _out_of_plane_strain;
  const VariableValue & _out_of_plane_strain_old;
};
