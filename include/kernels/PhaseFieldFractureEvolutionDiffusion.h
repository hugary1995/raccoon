//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernel.h"

class PhaseFieldFractureEvolutionDiffusion : public ADKernel
{
public:
  static InputParameters validParams();

  PhaseFieldFractureEvolutionDiffusion(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  /// Allen-Cahn interface coefficient computed from fracture properties
  const MaterialProperty<Real> & _kappa;

  /// Allen-Cahn mobility computed from fracture properties
  const MaterialProperty<Real> & _M;

  /// the coordinate system
  const Moose::CoordinateSystemType & _coord_sys;
};
