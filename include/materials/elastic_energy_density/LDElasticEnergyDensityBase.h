//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElasticEnergyDensityBase.h"

class LDElasticEnergyDensityBase : public ElasticEnergyDensityBase
{
public:
  static InputParameters validParams();

  LDElasticEnergyDensityBase(const InputParameters & params);

  // Compute the Mandel stress given the elastic deformation gradient. This is
  // d(elastic_energy_density)/d(Fe).
  virtual ADRankTwoTensor MandelStress(const ADRankTwoTensor & Fe) = 0;

protected:
  virtual void precomputeQpElasticEnergyDensity() override;

  /// @{ Dependent state variables
  /// The mechanical deformation gradient
  const ADMaterialProperty<RankTwoTensor> & _Fm;
  /// The plastic deformation gradient
  const ADMaterialProperty<RankTwoTensor> * _Fp;
  /// @}

  /// @{ Other quantities needed by the elastic energy density
  /// The elastic deformation gradient
  ADMaterialProperty<RankTwoTensor> & _Fe;
  /// @}

  /// @{ Derivatives of the elastic energy density
  ADMaterialProperty<RankTwoTensor> & _dpsie_dFm;
  ADMaterialProperty<RankTwoTensor> * _dpsie_dFp;
  /// @}
};
