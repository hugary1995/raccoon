//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElasticityBase.h"

class ElastoplasticityBase : public ElasticityBase
{
public:
  static InputParameters validParams();

  ElastoplasticityBase(const InputParameters & parameters);

  virtual ADRankTwoTensor MandelStress() const = 0;

  virtual ADReal resolvedStress()

      virtual ADReal resolvedStressDerivative() const = 0;

protected:
  /// The elastic strain
  const ADMaterialProperty<RankTwoTensor> & _elastic_strain;

  /// The plastic strain
  const ADMaterialProperty<RankTwoTensor> & _plastic_strain;

  /// The plastic flow direction
  const ADMaterialProperty<RankTwoTensor> & _Np;
};
