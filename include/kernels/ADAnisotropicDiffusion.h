//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernel.h"
#include "BaseNameInterface.h"

class ADAnisotropicDiffusion : public ADKernel, public BaseNameInterface
{
public:
  static InputParameters validParams();

  ADAnisotropicDiffusion(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  /// gradient of the coupled variable
  const ADVariableGradient & _grad_v;

  /// The anisotropic diffusivity
  const ADMaterialProperty<RankTwoTensor> & _D;
};
