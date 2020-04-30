//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"
#include "MaterialPropertyUserObject.h"

class MaterialPropertyUserObjectReaction : public ADKernelValue
{
public:
  static InputParameters validParams();

  MaterialPropertyUserObjectReaction(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  const MaterialPropertyUserObject & _uo;
};
