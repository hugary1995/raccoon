//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADTimeKernel.h"
#include "BaseNameInterface.h"

class ADPorousFlowTimeDerivative : public ADTimeKernel, public BaseNameInterface
{
public:
  static InputParameters validParams();

  ADPorousFlowTimeDerivative(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  // @{ The current and old values of porosity used to compute its time derivative.
  const ADMaterialProperty<Real> & _porosity;
  const MaterialProperty<Real> & _porosity_old;
  // @}

  /// The saturation
  const ADVariableValue & _S;

  /// The time derivative of the saturation
  const ADVariableValue & _S_dot;
};
