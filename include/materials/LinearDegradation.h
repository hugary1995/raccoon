//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "DegradationBase.h"

template <bool is_ad>
class LinearDegradationTempl : public DegradationBaseTempl<true>
{
public:
  static InputParameters validParams();

  LinearDegradationTempl(const InputParameters & parameters);

protected:
  virtual void computeDegradation() override;
};

typedef LinearDegradationTempl<true> LinearDegradation;
