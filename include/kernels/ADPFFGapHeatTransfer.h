//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"
#include "BaseNameInterface.h"

class ADPFFGapHeatTransfer : public ADKernelValue, public BaseNameInterface
{
public:
  static InputParameters validParams();

  ADPFFGapHeatTransfer(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  std::vector<const ADVariableValue *> _T_jump;

  const ADVariableGradient & _grad_phi;

  const ADMaterialProperty<Real> & _gapK;

  const ADVariableValue & _u_jump;

  const ADVariableGradient & _grad_d;

  const Real _min_gap;
};
