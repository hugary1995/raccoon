//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "LargeDeformationElasticityModel.h"

class HenckyIsotropicElasticity : public LargeDeformationElasticityModel
{
public:
  static InputParameters validParams();

  HenckyIsotropicElasticity(const InputParameters & parameters);

  virtual ADRankTwoTensor computeMandelStress(const ADRankTwoTensor & Fe) override;

  virtual ADRankTwoTensor computeMandelStressExponentialUpdate(const ADRankTwoTensor & Fe) override;

protected:
  /// The bulk modulus
  const ADMaterialProperty<Real> & _K;

  /// The shear modulus
  const ADMaterialProperty<Real> & _G;
};
