#ifndef SmallStrainDegradedPK2Stress_StrainSpectral_H
#define SmallStrainDegradedPK2Stress_StrainSpectral_H

#include "ADDegradedStressBase.h"

template <ComputeStage>
class SmallStrainDegradedPK2Stress_StrainSpectral;

declareADValidParams(SmallStrainDegradedPK2Stress_StrainSpectral);

template <ComputeStage compute_stage>
class SmallStrainDegradedPK2Stress_StrainSpectral : public ADDegradedStressBase<compute_stage>
{
public:
  SmallStrainDegradedPK2Stress_StrainSpectral(const InputParameters & parameters);

protected:
  virtual void computeQpStress() override;

  usingDegradedStressBaseMembers
};

#endif // SmallStrainDegradedPK2Stress_StrainSpectral_H
