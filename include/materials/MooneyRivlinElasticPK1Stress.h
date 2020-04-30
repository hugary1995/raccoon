//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADMaterial.h"

class MooneyRivlinElasticPK1Stress : public ADMaterial
{
public:
  static InputParameters validParams();

  MooneyRivlinElasticPK1Stress(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// base name of the stress
  const std::string _base_name;

  /// deformation gradient
  const ADMaterialProperty<RankTwoTensor> & _F;

  /// right Cauchy-Green strain
  const ADMaterialProperty<RankTwoTensor> & _C;

  /// material properties
  const MaterialProperty<Real> & _eta1;
  const MaterialProperty<Real> & _eta2;
  const MaterialProperty<Real> & _eta3;

  /// stress
  ADMaterialProperty<RankTwoTensor> & _stress;
  ADMaterialProperty<RankTwoTensor> & _cauchy_stress;
};
