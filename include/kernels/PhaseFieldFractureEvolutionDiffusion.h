//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelGrad.h"

// Forward Declarations
template <ComputeStage>
class PhaseFieldFractureEvolutionDiffusion;

declareADValidParams(PhaseFieldFractureEvolutionDiffusion);

template <ComputeStage compute_stage>
class PhaseFieldFractureEvolutionDiffusion : public ADKernelGrad<compute_stage>
{
public:
  static InputParameters validParams();

  PhaseFieldFractureEvolutionDiffusion(const InputParameters & parameters);

protected:
  virtual ADRealVectorValue precomputeQpResidual() override;
};
