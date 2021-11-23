//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernel.h"
#include "BaseNameInterface.h"

class Nitsche : public ADKernel, public BaseNameInterface
{
public:
  static InputParameters validParams();

  Nitsche(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  const ADVariableGradient & _grad_phi;

  const ADMaterialProperty<Real> & _wn;

  const ADMaterialProperty<Real> & _alpha;
};
