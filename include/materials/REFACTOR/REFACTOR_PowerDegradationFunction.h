//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "REFACTOR_DegradationFunctionBase.h"

/**
 * Predefined power degradation function
 */
class REFACTOR_PowerDegradationFunction : public REFACTOR_DegradationFunctionBase
{
public:
  static InputParameters validParams();

  REFACTOR_PowerDegradationFunction(const InputParameters & parameters);
};
