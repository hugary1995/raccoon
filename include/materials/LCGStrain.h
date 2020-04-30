//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADComputeStrainBase.h"

template <typename>
class RankTwoTensorTempl;
typedef RankTwoTensorTempl<Real> RankTwoTensor;
typedef RankTwoTensorTempl<DualReal> DualRankTwoTensor;

/**
 * LCGStrain defines a non-linear Green-Lagrange strain tensor
 */
class LCGStrain : public ADComputeStrainBase
{
public:
  static InputParameters validParams();
  LCGStrain(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// deformation gradient
  ADMaterialProperty<RankTwoTensor> & _F;
};
