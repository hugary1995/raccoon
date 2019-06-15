//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "GenericFunctionMaterial.h"

// Forward Declarations
class StationaryGenericFunctionMaterial;

template <>
InputParameters validParams<StationaryGenericFunctionMaterial>();

class StationaryGenericFunctionMaterial : public GenericFunctionMaterial
{
public:
  StationaryGenericFunctionMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;
};
