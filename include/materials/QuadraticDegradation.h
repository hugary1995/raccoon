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
  virtual void computeDegradation(ADReal &) override;

  usingDegradationBaseMembers
};
