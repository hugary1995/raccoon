//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernel.h"

#define usingCoefReactionMembers                                                                   \
  usingKernelMembers;                                                                              \
  using ADCoefReaction<compute_stage>::_coef;

// Forward Declarations
template <ComputeStage>
class ADCoefReaction;

declareADValidParams(ADCoefReaction);

template <ComputeStage compute_stage>
class ADCoefReaction : public ADKernel<compute_stage>
{
public:
  ADCoefReaction(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  const Real _coef;

  usingKernelMembers;
};
