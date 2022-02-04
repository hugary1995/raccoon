//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "PlasticDeformationBase.h"

class SDPlasticStrainBase : public SDPlasticDeformationBase
{
public:
  static InputParameters validParams();

  SDPlasticStrainBase(const InputParameters & params);

protected:
  virtual void initQpStatefulProperties() override;

  /// The mechanical strain
  const ADMaterialProperty<RankTwoTensor> & _mechanical_strain;

  // {@ The plastic strain
  ADMaterialProperty<RankTwoTensor> & _plastic_strain;
  const MaterialProperty<RankTwoTensor> & _plastic_strain_old;
  // @}
};
