//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"
#include "ADRankTwoTensorForward.h"

class LargeDeformationElasticityModel;
class LargeDeformationPlasticityModel;
class LargeDeformationViscoelasticityModel;

/**
 * ComputeLargeDeformationStress computes the stress given certain hyperelasticity and optionally a
 * plasticity model and a viscoelasticity model
 */
class ComputeLargeDeformationStress : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();

  ComputeLargeDeformationStress(const InputParameters & parameters);

  virtual void initialSetup() override;

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;

  /// The elasticity model
  LargeDeformationElasticityModel * _elasticity_model;

  /// The plasticity model
  LargeDeformationPlasticityModel * _plasticity_model;

  /// The viscoelasticity model
  LargeDeformationViscoelasticityModel * _viscoelasticity_model;

  // @{ The mechanical strain excluding eigen strains from the total strain
  const ADMaterialProperty<RankTwoTensor> & _Fm;
  const MaterialProperty<RankTwoTensor> * _Fm_old;
  // @}

  /// The stress
  ADMaterialProperty<RankTwoTensor> & _stress;
  const MaterialProperty<RankTwoTensor> & _stress_old;
  ADMaterialProperty<RankTwoTensor> & _stress_old_store;
};
