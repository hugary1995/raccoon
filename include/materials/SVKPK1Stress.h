//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADComputeStressBase.h"

template <ComputeStage>
class SVKPK1Stress;

declareADValidParams(SVKPK1Stress);

template <ComputeStage compute_stage>
class SVKPK1Stress : public ADComputeStressBase<compute_stage>
{
public:
  static InputParameters validParams();

  SVKPK1Stress(const InputParameters & parameters);

protected:
  virtual void computeQpStress() override;

  /// elasticity tensor
  const ADMaterialProperty(RankFourTensor) & _elasticity_tensor;

  /// deformation gradient
  const ADMaterialProperty(RankTwoTensor) & _F;

  /// Cauchy stress
  ADMaterialProperty(RankTwoTensor) * _cauchy_stress;

  usingComputeStressBaseMembers;
};
