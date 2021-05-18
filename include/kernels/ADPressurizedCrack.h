//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"
#include "BaseNameInterface.h"

class ADPressurizedCrack : public ADKernelValue, public BaseNameInterface
{
public:
  static InputParameters validParams();

  ADPressurizedCrack(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  const unsigned int _comp;
  const ADMaterialProperty<Real> & _p;
  const ADVariableGradient & _grad_d;
};
