//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADComputeStrainBase.h"

template <ComputeStage>
class RCGStrain;
template <typename>
class RankTwoTensorTempl;
typedef RankTwoTensorTempl<Real> RankTwoTensor;
typedef RankTwoTensorTempl<DualReal> DualRankTwoTensor;

declareADValidParams(RCGStrain);

/**
 * RCGStrain defines a non-linear Green-Lagrange strain tensor
 */
template <ComputeStage compute_stage>
class RCGStrain : public ADComputeStrainBase<compute_stage>
{
public:
  RCGStrain(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  ADMaterialProperty(RankTwoTensor) & _F;

  usingComputeStrainBaseMembers;
};

