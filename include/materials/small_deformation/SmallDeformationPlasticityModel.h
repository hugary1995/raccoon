//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "ADRankTwoTensorForward.h"

class SmallDeformationElasticityModel;

class SmallDeformationPlasticityModel : public Material
{
public:
  static InputParameters validParams();

  SmallDeformationPlasticityModel(const InputParameters & parameters);

  /// Set the current quadrature point
  void setQp(unsigned int qp) { _qp = qp; }

  /// Set the associated plasticity model
  void setElasticityModel(SmallDeformationElasticityModel * elasticity_model);

  /**
   * Update the stress and elastic strain if need to following the specified plastic flow
   * @param stress         The stress
   * @param elastic_strain The elastic strain
   */
  virtual void updateState(ADRankTwoTensor & stress, ADRankTwoTensor & elastic_strain) = 0;

  // @{ Retained as empty methods to avoid a warning from Material.C in framework. These methods are
  // unused in all inheriting classes and should not be overwritten.
  void resetQpProperties() final {}
  void resetProperties() final {}
  // @}

protected:
  virtual void initQpStatefulProperties() override;

  /// The optional plasticity model
  SmallDeformationElasticityModel * _elasticity_model;

  /// Base name optionally used as prefix to material tensor names
  const std::string _base_name;

  /// The elastic strain
  ADMaterialProperty<RankTwoTensor> & _plastic_strain;
};
