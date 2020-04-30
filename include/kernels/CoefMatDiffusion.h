//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelGrad.h"

class CoefMatDiffusion : public ADKernelGrad
{
public:
  static InputParameters validParams();

  CoefMatDiffusion(const InputParameters & parameters);

protected:
  virtual ADRealVectorValue precomputeQpResidual() override;

  const Real _coef;

  const MaterialProperty<Real> * _prop;
};
