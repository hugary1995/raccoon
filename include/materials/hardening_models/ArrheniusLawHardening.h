//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "PlasticHardeningModel.h"
#include "DerivativeMaterialPropertyNameInterface.h"

class ArrheniusLawHardening : public PlasticHardeningModel,
                              public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  ArrheniusLawHardening(const InputParameters & parameters);

  virtual ADReal plasticEnergy(const ADReal & ep, const unsigned int derivative = 0) override;

protected:
  // @{ The plastic energy parameters
  const ADMaterialProperty<Real> & _sigma_0;
  const ADMaterialProperty<Real> & _arrhenius_coef;
  const Real _eps;
  // @}

  /// Name of the phase-field variable
  const VariableName _d_name;

  // @{ Plastic energy density and its derivative w/r/t damage
  const MaterialPropertyName _psip_name;
  ADMaterialProperty<Real> & _psip;
  ADMaterialProperty<Real> & _psip_active;
  ADMaterialProperty<Real> & _dpsip_dd;
  // @}

  // @{ The degradation function and its derivative w/r/t damage
  const MaterialPropertyName _gp_name;
  const ADMaterialProperty<Real> & _gp;
  const ADMaterialProperty<Real> & _dgp_dd;
  // @}
};
