//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "DegradationBase.h"

class QuadraticDegradation : public DegradationBase
{
public:
  static InputParameters validParams();

  QuadraticDegradation(const InputParameters & parameters);

protected:
  virtual void computeDegradation() override;
};
