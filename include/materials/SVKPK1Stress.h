#ifndef SVKPK1Stress_H
#define SVKPK1Stress_H

#include "ADComputeStressBase.h"

template <ComputeStage>
class SVKPK1Stress;

declareADValidParams(SVKPK1Stress);

template <ComputeStage compute_stage>
class SVKPK1Stress : public ADComputeStressBase<compute_stage>
{
public:
  SVKPK1Stress(const InputParameters & parameters);

protected:
  virtual void computeQpStress() override;

  /// Elasticity tensor material property
  const ADMaterialProperty(RankFourTensor) & _elasticity_tensor;

  /// Elasticity tensor material property
  const ADMaterialProperty(RankTwoTensor) & _F;

  /// Cauchy stress
  ADMaterialProperty(RankTwoTensor) * _cauchy_stress;

  usingComputeStressBaseMembers;
};

#endif // SVKPK1Stress_H
