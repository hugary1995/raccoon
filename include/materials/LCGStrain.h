#ifndef LCGStrain_H
#define LCGStrain_H

#include "ADComputeStrainBase.h"

template <ComputeStage>
class LCGStrain;
template <typename>
class RankTwoTensorTempl;
typedef RankTwoTensorTempl<Real> RankTwoTensor;
typedef RankTwoTensorTempl<DualReal> DualRankTwoTensor;

declareADValidParams(LCGStrain);

/**
 * LCGStrain defines a non-linear Green-Lagrange strain tensor
 */
template <ComputeStage compute_stage>
class LCGStrain : public ADComputeStrainBase<compute_stage>
{
public:
  LCGStrain(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  ADMaterialProperty(RankTwoTensor) & _F;

  usingComputeStrainBaseMembers;
};

#endif // LCGStrain_H
