//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "ADRankTwoTensorForward.h"

class SmallDeformationElasticityModel;

/**
 * ComputeSmallDeformationStress computes the stress under small-strain assumptions
 */
class ComputeSmallDeformationStress : public Material
{
public:
  static InputParameters validParams();

  ComputeSmallDeformationStress(const InputParameters & parameters);

  virtual void initialSetup() override;

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;

  /// The elasticity model
  SmallDeformationElasticityModel * _elasticity_model;

  /// Base name of the material system
  const std::string _base_name;

  /// The mechanical strain excluding eigen strains from the total strain
  const ADMaterialProperty<RankTwoTensor> & _mechanical_strain;

  /// The stress
  ADMaterialProperty<RankTwoTensor> & _stress;
};
