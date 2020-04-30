//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADComputeStrainBase.h"

/**
 * GreenStrainRZ defines a non-linear Green-Lagrange strain tensor
 */
class GreenStrainRZ : public ADComputeStrainBase
{
public:
  static InputParameters validParams();

  GreenStrainRZ(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void initialSetup() override;
  virtual ADReal computeQpOutOfPlaneGradDisp();
  virtual void computeProperties() override;
  virtual void computeQpStrain();

  /// deformation gradient
  ADMaterialProperty<RankTwoTensor> & _F;
  const MaterialProperty<RankTwoTensor> & _F_old;
  ADMaterialProperty<RankTwoTensor> & _f;
};
