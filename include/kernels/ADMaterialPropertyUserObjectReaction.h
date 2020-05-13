//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"
#include "MaterialPropertyUserObject.h"

class ADMaterialPropertyUserObjectReaction : public ADKernelValue
{
public:
  static InputParameters validParams();

  ADMaterialPropertyUserObjectReaction(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  const ADMaterialPropertyUserObject & _uo;
};
