//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "DegradationBase.h"

template <bool is_ad>
class LorentzDegradationTempl : public DegradationBaseTempl<true>
{
public:
  static InputParameters validParams();

  LorentzDegradationTempl(const InputParameters & parameters);

protected:
  virtual void computeDegradation() override;

  /// mobility
  const GenericMaterialProperty<Real, is_ad> & _M;

  /// critical fracture energy
  const GenericMaterialProperty<Real, is_ad> & _b;

  /// shape parameter
  const Real & _p;

  /// initial slope of the local dissipation function
  const Real & _xi;

  /// coalescence coefficient
  const Real & _beta;
};

typedef LorentzDegradationTempl<true> LorentzDegradation;
