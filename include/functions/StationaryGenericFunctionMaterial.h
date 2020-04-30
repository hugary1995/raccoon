//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "GenericFunctionMaterial.h"

class StationaryGenericFunctionMaterial : public GenericFunctionMaterial
{
public:
  static InputParameters validParams();

  StationaryGenericFunctionMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;
};
