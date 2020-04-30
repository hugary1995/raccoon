//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADComputeStrainBase.h"

/**
 * RCGStrain defines a non-linear right Cauchy-Green strain tensor
 */
class RCGStrain : public ADComputeStrainBase
{
public:
  static InputParameters validParams();

  RCGStrain(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeProperties() override;
  virtual void computeQpStrain();

  /// deformation gradient
  ADMaterialProperty<RankTwoTensor> & _F;
};
