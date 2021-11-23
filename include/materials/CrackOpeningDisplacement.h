//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"

class CrackOpeningDisplacement : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();

  CrackOpeningDisplacement(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// The crack opening displacement
  ADMaterialProperty<Real> & _wn;

  /// The displacements
  std::vector<const ADVariableValue *> _disp;

  /// The gradient of phase field
  const ADVariableValue & _d;
  const ADVariableGradient & _grad_d;

  /// The gradient of level set
  const ADVariableGradient & _grad_phi;
};
