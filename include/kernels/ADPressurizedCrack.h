//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"

class ADPressurizedCrack : public ADKernelValue
{
public:
  static InputParameters validParams();

  ADPressurizedCrack(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  const unsigned int _comp;
  const ADMaterialProperty<Real> * _p_mat;
  const ADVariableValue * _p_var;
  const ADVariableValue & _d;
  const ADVariableGradient & _grad_d;
  const Real _xi;
};
