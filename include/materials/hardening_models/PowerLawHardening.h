//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "PlasticHardeningModel.h"
#include "DerivativeMaterialPropertyNameInterface.h"

class PowerLawHardening : public PlasticHardeningModel,
                          public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  PowerLawHardening(const InputParameters & parameters);

  virtual ADReal plasticEnergy(const ADReal & ep, const unsigned int derivative = 0) override;

protected:
  // @{ The plastic energy parameters
  const ADMaterialProperty<Real> & _sigma_y;
  const ADMaterialProperty<Real> & _n;
  const ADMaterialProperty<Real> & _ep0;
  // @}

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
