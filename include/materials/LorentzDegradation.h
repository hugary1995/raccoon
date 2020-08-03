//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "DegradationBase.h"

class LorentzDegradation : public DegradationBase
{
public:
  static InputParameters validParams();

  LorentzDegradation(const InputParameters & parameters);

protected:
  virtual void computeDegradation() override;

  /// mobility
  const ADMaterialProperty<Real> & _M;

  /// critical fracture energy
  const MaterialProperty<Real> & _b;

  /// shape parameter
  const Real & _p;

  /// initial slope of the local dissipation function
  const Real & _xi;

  /// coalescence coefficient
  const Real & _beta;
};
