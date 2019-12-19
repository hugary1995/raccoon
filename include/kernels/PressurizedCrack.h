//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"

// Forward Declarations
template <ComputeStage>
class PressurizedCrack;

declareADValidParams(PressurizedCrack);

template <ComputeStage compute_stage>
class PressurizedCrack : public ADKernelValue<compute_stage>
{
public:
  static InputParameters validParams();

  PressurizedCrack(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  const unsigned int _comp;
  const ADVariableValue & _p;
  const ADVariableGradient & _grad_d;

  usingKernelValueMembers;
};
