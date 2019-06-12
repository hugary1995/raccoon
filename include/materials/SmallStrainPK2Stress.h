//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADComputeStressBase.h"

template <ComputeStage>
class SmallStrainPK2Stress;

declareADValidParams(SmallStrainPK2Stress);

template <ComputeStage compute_stage>
class SmallStrainPK2Stress : public ADComputeStressBase<compute_stage>
{
public:
  SmallStrainPK2Stress(const InputParameters & parameters);

protected:
  virtual void computeQpStress() override;

  /// Elasticity tensor material property
  const ADMaterialProperty(RankFourTensor) & _elasticity_tensor;

  /// elastic energy name
  const MaterialPropertyName _E_el_name;

  /// positive elastic driving energy
  ADMaterialProperty(Real) & _E_el_pos;

  usingComputeStressBaseMembers;
};
