//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu
//* By: @Sina-av

#pragma once

#include "PlasticEnergyDensityBase.h"

class LinearHardeningPlasticEnergyDensity : public PlasticEnergyDensityBase
{
public:
  static InputParameters validParams();

  LinearHardeningPlasticEnergyDensity(const InputParameters & params);

  virtual ADReal flowStress(const ADReal & ep) override;

  virtual ADReal flowStressDerivative(const ADReal & ep) override;

protected:
  virtual void computeQpPlasticEnergyDensity() override;

  virtual void finalizeQpPlasticEnergyDensity() override;

  /// The yield stress
  const ADMaterialProperty<Real> & _sigma_y;

  /// The hardening modulus
  const ADMaterialProperty<Real> & _H;
};
