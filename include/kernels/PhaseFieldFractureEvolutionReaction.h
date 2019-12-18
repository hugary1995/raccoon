//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "PhaseFieldFractureEvolutionBase.h"

// Forward Declarations
template <ComputeStage>
class PhaseFieldFractureEvolutionReaction;

declareADValidParams(PhaseFieldFractureEvolutionReaction);

template <ComputeStage compute_stage>
class PhaseFieldFractureEvolutionReaction : public PhaseFieldFractureEvolutionBase<compute_stage>
{
public:
  static InputParameters validParams();

  PhaseFieldFractureEvolutionReaction(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  usingPhaseFieldFractureEvolutionBaseMembers;
};
