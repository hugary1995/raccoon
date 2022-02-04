//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "PlasticDeformationBase.h"

class LDPlasticDefgradBase : public LDPlasticDeformationBase
{
public:
  static InputParameters validParams();

  LDPlasticDefgradBase(const InputParameters & params);

protected:
  virtual void initQpStatefulProperties() override;

  /// The mechanical deformation gradient
  const ADMaterialProperty<RankTwoTensor> & _Fm;

  // {@ The plastic deformation gradient
  ADMaterialProperty<RankTwoTensor> & _Fp;
  const MaterialProperty<RankTwoTensor> & _Fp_old;
  // @}
};
