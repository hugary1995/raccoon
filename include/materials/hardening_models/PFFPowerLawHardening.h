//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "PowerLawHardening.h"
#include "DerivativeMaterialPropertyNameInterface.h"

class PFFPowerLawHardening : public PowerLawHardening,
                             public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  PFFPowerLawHardening(const InputParameters & parameters);

  virtual ADReal plasticEnergy(const ADReal & ep, const unsigned int derivative = 0) override;

protected:
  /// Name of the phase-field variable
  const VariableName _d_name;

  // @{ Plastic energy density and its derivative w/r/t damage
  const MaterialPropertyName _wp_name;
  ADMaterialProperty<Real> & _wp;
  ADMaterialProperty<Real> & _wp_active;
  ADMaterialProperty<Real> & _dwp_dd;
  // @}

  // @{ The degradation function and its derivative w/r/t damage
  const MaterialPropertyName _gp_name;
  const ADMaterialProperty<Real> & _gp;
  const ADMaterialProperty<Real> & _dgp_dd;
  // @}
};
