//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADCoefReaction.h"

// Forward Declarations
template <ComputeStage>
class DegradedCoefReaction;

declareADValidParams(DegradedCoefReaction);

template <ComputeStage compute_stage>
class DegradedCoefReaction : public ADCoefReaction<compute_stage>
{
public:
  DegradedCoefReaction(const InputParameters & parameters);

protected:
  virtual ADResidual computeQpResidual() override;

  /// degradation
  const ADMaterialProperty(Real) & _g;

  usingCoefReactionMembers;
};
