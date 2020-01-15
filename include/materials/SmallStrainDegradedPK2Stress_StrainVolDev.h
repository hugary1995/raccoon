//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADDegradedStressBase.h"

template <ComputeStage>
class SmallStrainDegradedPK2Stress_StrainVolDev;

declareADValidParams(SmallStrainDegradedPK2Stress_StrainVolDev);

template <ComputeStage compute_stage>
class SmallStrainDegradedPK2Stress_StrainVolDev : public ADDegradedStressBase<compute_stage>
{
public:
  static InputParameters validParams();

  SmallStrainDegradedPK2Stress_StrainVolDev(const InputParameters & parameters);

protected:
  virtual void computeQpStress() override;

  usingDegradedStressBaseMembers
};
