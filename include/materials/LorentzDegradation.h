//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "DegradationBase.h"

// Forward Declarations
template <ComputeStage>
class LorentzDegradation;

declareADValidParams(LorentzDegradation);

template <ComputeStage compute_stage>
class LorentzDegradation : public DegradationBase<compute_stage>
{
public:
  LorentzDegradation(const InputParameters & parameters);

protected:
  virtual void computeDegradation() override;

  /// mobility
  const MaterialProperty<Real> & _M;

  /// fracture energy barrier
  const MaterialProperty<Real> & _b;

  /// shape parameter
  const Real & _p;

  usingDegradationBaseMembers
};
