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

  /// Allen-Cahn interface coefficient computed from fracture properties
  const ADMaterialProperty<Real> & _kappa;

  /// Allen-Cahn mobility computed from fracture properties
  const ADMaterialProperty<Real> & _M;

  /// the coordinate system
  const Moose::CoordinateSystemType & _coord_sys;
};
