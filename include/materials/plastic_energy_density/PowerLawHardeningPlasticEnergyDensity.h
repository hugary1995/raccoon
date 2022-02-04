//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu
//* By: @Sina-av

#pragma once

#include "PlasticEnergyDensityBase.h"

class PowerLawHardeningPlasticEnergyDensity : public PlasticEnergyDensityBase
{
public:
  static InputParameters validParams();

  PowerLawHardeningPlasticEnergyDensity(const InputParameters & params);

  virtual ADReal flowStress(const ADReal & ep) override;

  virtual ADReal flowStressDerivative(const ADReal & ep) override;

protected:
  virtual void computeQpPlasticEnergyDensity() override;

  virtual void finalizeQpPlasticEnergyDensity() override;

  /// The yield stress
  const ADMaterialProperty<Real> & _sigma_y;

  /// The hardening exponent
  const ADMaterialProperty<Real> & _n;

  /// The reference plastic strain
  const ADMaterialProperty<Real> & _ep0;
};
