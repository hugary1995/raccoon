//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernel.h"
#include "MaterialPropertyUserObject.h"

class ADDarcyDiffusion : public ADKernel
{
public:
  static InputParameters validParams();

  ADDarcyDiffusion(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  const ADMaterialProperty<RankTwoTensor> & _permeability;

  const ADMaterialProperty<Real> & _density;

};
