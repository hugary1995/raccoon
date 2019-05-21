//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADComputeStressBase.h"

template <ComputeStage>
class CNHDegradedPK1Stress;

declareADValidParams(CNHDegradedPK1Stress);

/**
 * CNHDegradedPK1Stress computes the stress following linear elasticity theory (small
 * strains)
 */
template <ComputeStage compute_stage>
class CNHDegradedPK1Stress : public ADComputeStressBase<compute_stage>
{
public:
  CNHDegradedPK1Stress(const InputParameters & parameters);

protected:
  virtual void computeQpStress() override;

  /// Elasticity tensor material property
  const ADMaterialProperty(RankFourTensor) & _elasticity_tensor;

  /// deformation gradient
  const ADMaterialProperty(RankTwoTensor) & _F;

  /// degradation name
  const MaterialPropertyName _g_name;

  /// degradation
  const ADMaterialProperty(Real) & _g;

  /// whether to enforce irreversibility using history
  const bool _history;

  /// elastic energy name
  const MaterialPropertyName _E_el_name;

  /// positive elastic driving energy
  ADMaterialProperty(Real) & _E_el_pos;

  /// old value of the driving energy
  const MaterialProperty<Real> & _E_el_pos_old;

  usingComputeStressBaseMembers;
};

