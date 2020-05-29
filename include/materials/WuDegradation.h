//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "DegradationBase.h"

class WuDegradation : public DegradationBase
{
public:
  static InputParameters validParams();

  WuDegradation(const InputParameters & parameters);

protected:
  virtual void computeDegradation() override;

  /// mobility
  const ADMaterialProperty<Real> & _M;

  /// critical fracture energy
  const MaterialProperty<Real> & _b;

  /// shape parameters
  const Real & _a2;
  const Real & _a3;

  /// initial slope of the local dissipation function
  const Real & _xi;
};
