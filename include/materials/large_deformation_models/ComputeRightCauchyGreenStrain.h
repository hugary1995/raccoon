//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"
#include "ADRankTwoTensorForward.h"

/**
 * This class computes the deformation gradient
 */
class ComputeRightCauchyGreenStrain : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();

  ComputeRightCauchyGreenStrain(const InputParameters & parameters);

  void computeQpProperties() override;

protected:
  /// The total deformation gradient
  const ADMaterialProperty<RankTwoTensor> & _F;

  /// The mechanical deformation gradient
  const ADMaterialProperty<RankTwoTensor> & _Fm;

  /// The total strain
  ADMaterialProperty<RankTwoTensor> & _total_strain;

  /// The mechanical strain
  ADMaterialProperty<RankTwoTensor> & _mechanical_strain;

  /// Whether to compute the logarithmic strain
  const bool _logarithmic;
};
