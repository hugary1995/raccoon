//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "DegradationBase.h"

template <bool is_ad>
class QuasiLinearDegradationTempl : public DegradationBaseTempl<true>
{
public:
  static InputParameters validParams();

  QuasiLinearDegradationTempl(const InputParameters & parameters);

protected:
  virtual void computeDegradation() override;

  /// mobility
  const GenericMaterialProperty<Real, is_ad> & _M;

  /// critical fracture energy
  const GenericMaterialProperty<Real, is_ad> & _b;

  /// derivative of local dissipation function at d = 0
  const Real & _xi;
};

typedef QuasiLinearDegradationTempl<true> QuasiLinearDegradation;
