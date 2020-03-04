//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "DegradationBase.h"

// Forward Declarations
template <ComputeStage>
class LinearDegradation;

declareADValidParams(LinearDegradation);

template <ComputeStage compute_stage>
class LinearDegradation : public DegradationBase<compute_stage>
{
public:
  static InputParameters validParams();

  LinearDegradation(const InputParameters & parameters);

protected:
  virtual void computeDegradation() override;

  usingDegradationBaseMembers
};
