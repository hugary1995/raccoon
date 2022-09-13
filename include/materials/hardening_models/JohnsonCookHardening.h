//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "PlasticHardeningModel.h"
#include "DerivativeMaterialPropertyNameInterface.h"

class JohnsonCookHardening : public PlasticHardeningModel,
                             public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  JohnsonCookHardening(const InputParameters & parameters);

  virtual ADReal initialGuess(const ADReal & effective_trial_stress) override;
  virtual ADReal plasticEnergy(const ADReal & ep, const unsigned int derivative = 0) override;
  virtual ADReal plasticDissipation(const ADReal & delta_ep,
                                    const ADReal & ep,
                                    const unsigned int derivative) override;
  virtual ADReal thermalConjugate(const ADReal & ep) override;

protected:
  // @{ The plastic energy parameters
  const ADMaterialProperty<Real> & _sigma_0;
  const Real _n;
  const Real _m;
  const Real _ep0;
  const Real _epdot0;
  const Real _T0;
  // const ADVariableValue & _T;
  const VariableValue & _T;
  const Real _tqf;
  const ADMaterialProperty<Real> & _A;
  const Real _B;
  const Real _C;
  const Real _Tm;
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

private:
  ADReal temperatureDependence();
};
