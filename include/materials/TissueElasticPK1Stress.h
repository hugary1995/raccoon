//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADMaterial.h"

class TissueElasticPK1Stress : public ADMaterial
{
public:
  static InputParameters validParams();

  TissueElasticPK1Stress(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// base name of the stress
  const std::string _base_name;

  /// deformation gradient
  const ADMaterialProperty<RankTwoTensor> & _F;

  /// right Cauchy-Green strain
  const ADMaterialProperty<RankTwoTensor> & _C;

  /// material properties
  const MaterialProperty<Real> & _k1;
  const MaterialProperty<Real> & _k2;
  const MaterialProperty<RankTwoTensor> & _M;

  /// stress
  ADMaterialProperty<RankTwoTensor> & _stress;
  ADMaterialProperty<RankTwoTensor> & _cauchy_stress;

private:
  ADReal smoothRamp(ADReal x, Real s = 0.1);
  ADReal dSmoothRamp(ADReal x, Real s = 0.1);
};
