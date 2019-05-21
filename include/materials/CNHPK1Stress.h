#ifndef CNHPK1Stress_H
#define CNHPK1Stress_H

#include "ADComputeStressBase.h"

#define usingCNHPK1StressMembers                                                                   \
  usingComputeStressBaseMembers;                                                                   \
  using CNHPK1Stress<compute_stage>::_elasticity_tensor;                                           \
  using CNHPK1Stress<compute_stage>::_F;

template <ComputeStage>
class CNHPK1Stress;

declareADValidParams(CNHPK1Stress);

/**
 * CNHPK1Stress computes the stress following linear elasticity theory (small
 * strains)
 */
template <ComputeStage compute_stage>
class CNHPK1Stress : public ADComputeStressBase<compute_stage>
{
public:
  CNHPK1Stress(const InputParameters & parameters);

protected:
  virtual void computeQpStress() override;

  /// Elasticity tensor material property
  const ADMaterialProperty(RankFourTensor) & _elasticity_tensor;

  /// Elasticity tensor material property
  const ADMaterialProperty(RankTwoTensor) & _F;

  usingComputeStressBaseMembers;
};

#endif // CNHPK1Stress_H
