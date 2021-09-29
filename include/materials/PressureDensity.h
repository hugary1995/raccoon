//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"
#include "DerivativeMaterialPropertyNameInterface.h"

class PressureDensity : public Material,
                        public BaseNameInterface,
                        public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  PressureDensity(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// The effective pressure density
  ADMaterialProperty<Real> & _p_density;

  /// The gradient of phase field
  const ADVariableGradient & _grad_d;

  /// The applied pressure
  const ADMaterialProperty<Real> & _p;

  /// The derivative of the indicator function w.r.t. the phase field
  const ADMaterialProperty<Real> & _dI_dd;
};
