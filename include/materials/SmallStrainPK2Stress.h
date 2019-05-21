#ifndef SmallStrainPK2Stress_H
#define SmallStrainPK2Stress_H

#include "ADComputeStressBase.h"

template <ComputeStage>
class SmallStrainPK2Stress;

declareADValidParams(SmallStrainPK2Stress);

template <ComputeStage compute_stage>
class SmallStrainPK2Stress : public ADComputeStressBase<compute_stage>
{
public:
  SmallStrainPK2Stress(const InputParameters & parameters);

protected:
  virtual void computeQpStress() override;

  /// Elasticity tensor material property
  const ADMaterialProperty(RankFourTensor) & _elasticity_tensor;

  usingComputeStressBaseMembers;
};

#endif // SmallStrainPK2Stress_H
