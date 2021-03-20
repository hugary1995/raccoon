//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernel.h"

class ADDarcyTimeDerivative : public ADKernelValue
{
public:
  static InputParameters validParams();

  ADDarcyTimeDerivative(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  const ADMaterialProperty<Real> & _porosity;

  const MaterialProperty<Real> & _porosity_old;

  const ADMaterialProperty<Real> & _density;

  const MaterialProperty<Real> & _density_old;

};
