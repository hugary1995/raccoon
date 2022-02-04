//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "SDElasticEnergyDensityBase.h"

class SDIsotropicElasticEnergyDensity : public SDElasticEnergyDensityBase
{
public:
  static InputParameters validParams();

  SDIsotropicElasticEnergyDensity(const InputParameters & parameters);

  virtual ADRankTwoTensor MandelStress(const ADRankTwoTensor & strain) override;

protected:
  virtual void computeQpElasticEnergyDensity() override;

  virtual void finalizeQpElasticEnergyDensity() override;

  /// The bulk modulus
  const ADMaterialProperty<Real> & _K;

  /// The shear modulus
  const ADMaterialProperty<Real> & _G;
};
