//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADTimeDerivative.h"

class ADSPFPressureTimeDerivative : public ADTimeDerivative
{
public:
  static InputParameters validParams();

  ADSPFPressureTimeDerivative(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  const MaterialProperty<Real> & _M;
};
