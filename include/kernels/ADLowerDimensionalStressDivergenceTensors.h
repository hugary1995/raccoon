//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADStressDivergenceTensors.h"

class ADLowerDimensionalStressDivergenceTensors : public ADStressDivergenceTensors
{
public:
  static InputParameters validParams();

  ADLowerDimensionalStressDivergenceTensors(const InputParameters & parameters);

protected:
  ADReal computeQpResidual() override;

  /// Thickness of the lower dimensional block
  const ADMaterialProperty<Real> & _thickness;

  /// Coordinate transformation matrix
  const MaterialProperty<RankTwoTensor> & _Q;
};
