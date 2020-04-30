//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADMaterial.h"

class IncompressibilityPenaltyElasticPK1Stress : public ADMaterial
{
public:
  static InputParameters validParams();

  IncompressibilityPenaltyElasticPK1Stress(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// base name of the stress
  const std::string _base_name;

  /// deformation gradient
  const ADMaterialProperty<RankTwoTensor> & _F;

  /// right Cauchy-Green strain
  const ADMaterialProperty<RankTwoTensor> & _C;

  /// model parameters
  const Real _epsilon1;
  const Real _epsilon2;

  /// stress
  ADMaterialProperty<RankTwoTensor> & _stress;
  ADMaterialProperty<RankTwoTensor> & _cauchy_stress;
};
