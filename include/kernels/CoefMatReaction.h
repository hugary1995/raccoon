//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"

// Forward Declarations
template <ComputeStage>
class CoefMatReaction;

declareADValidParams(CoefMatReaction);

template <ComputeStage compute_stage>
class CoefMatReaction : public ADKernelValue<compute_stage>
{
public:
  static InputParameters validParams();

  CoefMatReaction(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  const Real _coef;

  const ADMaterialProperty(Real) * _prop;

  usingKernelValueMembers;
};
