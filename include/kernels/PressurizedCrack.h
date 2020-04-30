//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"

class PressurizedCrack : public ADKernelValue
{
public:
  static InputParameters validParams();

  PressurizedCrack(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  const unsigned int _comp;
  const MaterialProperty<Real> * _p_mat;
  const VariableValue * _p_var;
  const ADVariableGradient & _grad_d;
};
