//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADComputeStressBase.h"

#define usingCNHElasticPK1StressMembers                                                                   \
  usingComputeStressBaseMembers;                                                                   \
  using CNHElasticPK1Stress<compute_stage>::_elasticity_tensor;                                           \
  using CNHElasticPK1Stress<compute_stage>::_F;

template <ComputeStage>
class CNHElasticPK1Stress;

declareADValidParams(CNHElasticPK1Stress);

template <ComputeStage compute_stage>
class CNHElasticPK1Stress : public ADComputeStressBase<compute_stage>
{
public:
  static InputParameters validParams();

  CNHElasticPK1Stress(const InputParameters & parameters);

protected:
  virtual void computeQpStress() override;

  /// Elasticity tensor material property
  const ADMaterialProperty(RankFourTensor) & _elasticity_tensor;

  /// deformation gradient
  const ADMaterialProperty(RankTwoTensor) & _F;

  usingComputeStressBaseMembers;
};
