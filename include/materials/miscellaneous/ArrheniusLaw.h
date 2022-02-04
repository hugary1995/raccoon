//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"
#include "DerivativeMaterialPropertyNameInterface.h"

class ArrheniusLaw : public Material,
                     public BaseNameInterface,
                     public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  ArrheniusLaw(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  const std::string _arrhenius_coef_name;

  /// The Arrhenius coefficient
  ADMaterialProperty<Real> & _arrhenius_coef;

  /// The derivative of the Arrhenius coefficient W.R.T. the temperature
  ADMaterialProperty<Real> & _darrhenius_coef_dT;

  /// The activation energy
  const ADMaterialProperty<Real> & _Q;

  /// The ideal gas constant
  const Real _R;

  /// The temperature
  const ADVariableValue & _T;
};
