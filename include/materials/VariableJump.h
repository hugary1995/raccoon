//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"

class VariableJump : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();

  VariableJump(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// The variable jump
  ADMaterialProperty<RealVectorValue> & _wn;

  /// The variable
  const ADVariableValue & _u;

  /// The gradient of phase field
  const ADVariableValue & _d;
  const ADVariableGradient & _grad_d;

  /// The levelset
  const ADVariableGradient & _grad_phi;
};
