//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "ADRankTwoTensorForward.h"

class LargeDeformationPlasticityModel;

class LargeDeformationElasticityModel : public Material
{
public:
  static InputParameters validParams();

  LargeDeformationElasticityModel(const InputParameters & parameters);

  /// Set the current quadrature point
  virtual void setQp(unsigned int qp);

  /// Set the associated plasticity model
  virtual void setPlasticityModel(LargeDeformationPlasticityModel * plasticity_model);

  /**
   * Compute the stress given the mechanical deformation gradient. Also performs the plasticity
   * update, if any.
   * @param Fm      The mechanical deformation gradient
   * @param stress  The stress
   */
  virtual void updateState(const ADRankTwoTensor & Fm, ADRankTwoTensor & stress);

  /**
   * Compute the Mandel stress
   * @param Fe The given elastic deformation gradient
   * @return The computed stress given the deformation gradient and the constitutive relation
   */
  virtual ADRankTwoTensor computeMandelStress(const ADRankTwoTensor & Fe) = 0;

  /**
   * Exponentiate the deformation gradient and then compute the Mandel stress
   * @param Fe The given elastic deformation gradient to be exponentiated
   * @return The computed stress given the deformation gradient and the constitutive relation
   */
  virtual ADRankTwoTensor computeMandelStressExponentialUpdate(const ADRankTwoTensor & /*Fe*/)
  {
    mooseError("Exponential plasticity update is not supported for this model.");
  };

  /**
   * Compute the Cauchy stress given the elastic deformation gradient
   * @param Fe The given elastic deformation gradient
   * @return The computed stress given the deformation gradient and the constitutive relation
   */
  virtual ADRankTwoTensor computeCauchyStress(const ADRankTwoTensor & Fe);

  // @{ Retained as empty methods to avoid a warning from Material.C in framework. These methods are
  // unused in all inheriting classes and should not be overwritten.
  void resetQpProperties() final {}
  void resetProperties() final {}
  // @}

protected:
  virtual void initQpStatefulProperties() override;

  /// The optional plasticity model
  LargeDeformationPlasticityModel * _plasticity_model;

  /// Base name optionally used as prefix to material tensor names
  const std::string _base_name;

  /// The elastic strain
  ADMaterialProperty<RankTwoTensor> & _Fe;
};
