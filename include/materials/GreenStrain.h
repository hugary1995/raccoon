#ifndef GreenStrain_H
#define GreenStrain_H

#include "ADComputeStrainBase.h"

template <ComputeStage>
class GreenStrain;
template <typename>
class RankTwoTensorTempl;
typedef RankTwoTensorTempl<Real> RankTwoTensor;
typedef RankTwoTensorTempl<DualReal> DualRankTwoTensor;

declareADValidParams(GreenStrain);

/**
 * GreenStrain defines a non-linear Green-Lagrange strain tensor
 */
template <ComputeStage compute_stage>
class GreenStrain : public ADComputeStrainBase<compute_stage>
{
public:
  GreenStrain(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  ADMaterialProperty(RankTwoTensor) & _F;

  usingComputeStrainBaseMembers;
};

#endif // GreenStrain_H
