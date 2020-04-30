//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADDegradedElasticStressBase.h"

class SmallStrainDegradedElasticPK2Stress_NoSplit : public ADDegradedElasticStressBase
{
public:
  static InputParameters validParams();

  SmallStrainDegradedElasticPK2Stress_NoSplit(const InputParameters & parameters);

protected:
  virtual void computeQpStress() override;
};
