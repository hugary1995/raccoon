//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElementIntegralPostprocessor.h"
#include "BaseNameInterface.h"
#include "DerivativeMaterialPropertyNameInterface.h"

class EffectivePressure : public ElementIntegralPostprocessor,
                          public BaseNameInterface,
                          public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  EffectivePressure(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral() override;

  /// The pressure
  const ADMaterialProperty<Real> & _p;

  /// Gradient of phase field
  const VariableGradient & _grad_d;

  const VariableName _d_name;
  const MaterialPropertyName _I_name;
  const ADMaterialProperty<Real> & _dI_dd;
};
