//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "DegradationFunctionBase.h"

/**
 * Predefined power degradation function
 */
class PowerDegradationFunction : public DegradationFunctionBase
{
public:
  static InputParameters validParams();

  PowerDegradationFunction(const InputParameters & parameters);
};
