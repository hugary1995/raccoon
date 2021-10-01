//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"

class CrackSurfaceDensity : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();

  CrackSurfaceDensity(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// The gradient of phase field
  const ADVariableGradient & _grad_d;

  /// The normalization constant
  const ADMaterialProperty<Real> & _c0;

  /// The phase-field regularization length
  const ADMaterialProperty<Real> & _l;

  /// Crack geometric function
  const ADMaterialProperty<Real> & _alpha;

  /// The crack surface density
  ADMaterialProperty<Real> & _gamma;
};
