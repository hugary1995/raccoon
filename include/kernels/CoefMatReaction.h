//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"

class CoefMatReaction : public ADKernelValue
{
public:
  static InputParameters validParams();

  CoefMatReaction(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  const Real _coef;

  const MaterialProperty<Real> * _prop;
};
