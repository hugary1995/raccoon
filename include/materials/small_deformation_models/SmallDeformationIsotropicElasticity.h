//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "SmallDeformationElasticityModel.h"

class SmallDeformationIsotropicElasticity : public SmallDeformationElasticityModel
{
public:
  static InputParameters validParams();

  SmallDeformationIsotropicElasticity(const InputParameters & parameters);

  virtual ADRankTwoTensor computeStress(const ADRankTwoTensor & strain) override;

protected:
  /// The bulk modulus
  const ADMaterialProperty<Real> & _K;

  /// The shear modulus
  const ADMaterialProperty<Real> & _G;
};
