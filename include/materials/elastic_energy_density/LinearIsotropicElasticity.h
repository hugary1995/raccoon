//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElasticityBase.h"

class LinearIsotropicElasticity : public ElasticityBase
{
public:
  static InputParameters validParams();

  LinearIsotropicElasticity(const InputParameters & parameters);

  virtual ADRankTwoTensor stress() override;

protected:
  virtual void computeQpElasticEnergyDensity() override;

  /// The bulk modulus
  const ADMaterialProperty<Real> & _K;

  /// The shear modulus
  const ADMaterialProperty<Real> & _G;
};
