//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernel.h"
#include "MaterialPropertyUserObject.h"

class ADMaterialPropertyUserObjectDiffusion : public ADKernel
{
public:
  static InputParameters validParams();

  ADMaterialPropertyUserObjectDiffusion(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  const ADMaterialPropertyUserObject & _uo;

  const Real _coef;

  /// the coordinate system
  const Moose::CoordinateSystemType & _coord_sys;
};
