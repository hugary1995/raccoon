//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernel.h"

class ADPFFDiffusion : public ADKernel
{
public:
  static InputParameters validParams();

  ADPFFDiffusion(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  /// The normalization constant
  const ADMaterialProperty<Real> & _c0;

  /// The regularization length
  const ADMaterialProperty<Real> & _l;

  /// the coordinate system
  const Moose::CoordinateSystemType & _coord_sys;
};
