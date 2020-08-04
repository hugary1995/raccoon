//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "DegradationBase.h"

template <bool is_ad>
class WuDegradationTempl : public DegradationBaseTempl<true>
{
public:
  static InputParameters validParams();

  WuDegradationTempl(const InputParameters & parameters);

protected:
  virtual void computeDegradation() override;

  /// mobility
  const GenericMaterialProperty<Real, is_ad> & _M;

  /// critical fracture energy
  const GenericMaterialProperty<Real, is_ad> & _b;

  /// shape parameters
  const Real & _a2;
  const Real & _a3;

  /// initial slope of the local dissipation function
  const Real & _xi;
};

typedef WuDegradationTempl<true> WuDegradation;
