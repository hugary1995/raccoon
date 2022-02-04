//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElasticEnergyDensityBase.h"

class SDElasticEnergyDensityBase : public ElasticEnergyDensityBase
{
public:
  static InputParameters validParams();

  SDElasticEnergyDensityBase(const InputParameters & params);

  // Compute the Mandel stress given the elastic strain. This is
  // d(elastic_energy_density)/d(elastic_strain).
  virtual ADRankTwoTensor MandelStress(const ADRankTwoTensor & strain) = 0;

protected:
  virtual void precomputeQpElasticEnergyDensity() override;

  /// @{ Dependent state variables
  /// The mechanical strain
  const ADMaterialProperty<RankTwoTensor> & _mechanical_strain;
  /// The plastic strain
  const ADMaterialProperty<RankTwoTensor> * _plastic_strain;
  /// @}

  /// @{ Other quantities needed by the elastic energy density
  /// The elastic strain
  ADMaterialProperty<RankTwoTensor> & _elastic_strain;
  /// @}

  /// @{ Derivatives of the elastic energy density
  ADMaterialProperty<RankTwoTensor> & _dpsie_dmechanical_strain;
  ADMaterialProperty<RankTwoTensor> * _dpsie_dplastic_strain;
  /// @}
};
