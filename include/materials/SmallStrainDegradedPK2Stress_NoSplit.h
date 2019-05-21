#ifndef SmallStrainDegradedPK2Stress_NoSplit_H
#define SmallStrainDegradedPK2Stress_NoSplit_H

#include "ADDegradedStressBase.h"

template <ComputeStage>
class SmallStrainDegradedPK2Stress_NoSplit;

declareADValidParams(SmallStrainDegradedPK2Stress_NoSplit);

template <ComputeStage compute_stage>
class SmallStrainDegradedPK2Stress_NoSplit : public ADDegradedStressBase<compute_stage>
{
public:
  SmallStrainDegradedPK2Stress_NoSplit(const InputParameters & parameters);

protected:
  virtual void computeQpStress() override;

  usingDegradedStressBaseMembers
};

#endif // SmallStrainDegradedPK2Stress_NoSplit_H
