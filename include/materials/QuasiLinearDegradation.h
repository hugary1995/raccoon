//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "DegradationBase.h"

class QuasiLinearDegradation : public DegradationBase
{
public:
  static InputParameters validParams();

  QuasiLinearDegradation(const InputParameters & parameters);

protected:
  virtual void computeDegradation() override;

  /// mobility
  const MaterialProperty<Real> & _M;

  /// critical fracture energy
  const MaterialProperty<Real> & _b;
};
