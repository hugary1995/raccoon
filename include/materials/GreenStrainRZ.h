//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADComputeStrainBase.h"

template <ComputeStage>
class GreenStrainRZ;

declareADValidParams(GreenStrainRZ);

/**
 * GreenStrainRZ defines a non-linear Green-Lagrange strain tensor
 */
template <ComputeStage compute_stage>
class GreenStrainRZ : public ADComputeStrainBase<compute_stage>
{
public:
  static InputParameters validParams();

  GreenStrainRZ(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void initialSetup() override;
  virtual ADReal computeQpOutOfPlaneGradDisp();
  virtual void computeQpProperties() override;

  /// deformation gradient
  ADMaterialProperty(RankTwoTensor) & _F;

  usingComputeStrainBaseMembers;
};
