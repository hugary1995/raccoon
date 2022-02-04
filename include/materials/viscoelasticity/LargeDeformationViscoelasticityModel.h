//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"
#include "ADRankTwoTensorForward.h"

class LargeDeformationViscoelasticityModel : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();

  LargeDeformationViscoelasticityModel(const InputParameters & parameters);

  /// Set the current quadrature point
  virtual void setQp(unsigned int qp);

  /**
   * Compute the Cauchy stress given the mechanical deformation gradient
   * @param Fm The given mechanical deformation gradient
   * @return The computed stress given the deformation gradient and the constitutive relation
   */
  virtual ADRankTwoTensor computeCauchyStress(const ADRankTwoTensor & Fm,
                                              const RankTwoTensor & Fm_old) = 0;

  // @{ Retained as empty methods to avoid a warning from Material.C in framework. These methods are
  // unused in all inheriting classes and should not be overwritten.
  void resetQpProperties() final {}
  void resetProperties() final {}
  // @}
};
