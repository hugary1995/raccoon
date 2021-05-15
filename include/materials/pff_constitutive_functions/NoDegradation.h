//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "DegradationFunctionBase.h"

/**
 * Dummy definition of no degradation
 */
class NoDegradation : public DegradationFunctionBase
{
public:
  static InputParameters validParams();

  NoDegradation(const InputParameters & parameters);
};
