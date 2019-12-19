//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"

// Forward Declarations
template <ComputeStage>
class PhaseFieldFractureEvolutionPressure;

declareADValidParams(PhaseFieldFractureEvolutionPressure);

template <ComputeStage compute_stage>
class PhaseFieldFractureEvolutionPressure : public ADKernelValue<compute_stage>
{
public:
  static InputParameters validParams();

  PhaseFieldFractureEvolutionPressure(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  const ADVariableValue & _p;
  const unsigned int _ndisp;
  std::vector<const ADVariableGradient *> _grad_disp;

  usingKernelValueMembers;
};
