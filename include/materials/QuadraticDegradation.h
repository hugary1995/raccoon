//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "DegradationBase.h"

// Forward Declarations
template <ComputeStage>
class QuadraticDegradation;

declareADValidParams(QuadraticDegradation);

template <ComputeStage compute_stage>
class QuadraticDegradation : public DegradationBase<compute_stage>
{
public:
  QuadraticDegradation(const InputParameters & parameters);

protected:
  virtual void computeDegradation() override;

  usingDegradationBaseMembers
};
