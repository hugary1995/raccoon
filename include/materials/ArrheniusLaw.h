//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"

class ArrheniusLaw : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();

  ArrheniusLaw(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// The Arrhenius coefficient
  ADMaterialProperty<Real> & _arrhenius_coef;

  /// The activation energy
  const ADMaterialProperty<Real> & _Q;

  /// The ideal gas constant
  const Real _R;

  /// The temperature
  const ADVariableValue & _T;
};
