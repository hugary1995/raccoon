//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADTimeKernel.h"

class ADPorousFlowTimeDerivative : public ADTimeKernel
{
public:
  static InputParameters validParams();

  ADPorousFlowTimeDerivative(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  const ADMaterialProperty<Real> & _porosity;
  const MaterialProperty<Real> & _porosity_old;

  const ADVariableValue & _S;
  const ADVariableValue & _S_dot;
};
