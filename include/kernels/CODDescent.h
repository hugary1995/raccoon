//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernel.h"
#include "BaseNameInterface.h"

class CODDescent : public ADKernel, public BaseNameInterface
{
public:
  static InputParameters validParams();

  CODDescent(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  const ADVariableGradient & _grad_d;

  const ADVariableGradient & _grad_w;
};
