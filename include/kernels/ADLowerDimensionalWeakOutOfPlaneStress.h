//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"
#include "BaseNameInterface.h"
#include "Function.h"

class ADLowerDimensionalWeakOutOfPlaneStress : public ADKernelValue, public BaseNameInterface
{
public:
  static InputParameters validParams();

  ADLowerDimensionalWeakOutOfPlaneStress(const InputParameters & parameters);

protected:
  ADReal precomputeQpResidual() override;

  /// The out of plane stress
  const Function & _out_of_plane_stress;

  /// The stress tensor that provides the out-of-plane stress
  const ADMaterialProperty<RankTwoTensor> & _stress;

  /// The direction of the out-of-plane strain variable
  const MaterialProperty<RankTwoTensor> & _Q;
};
