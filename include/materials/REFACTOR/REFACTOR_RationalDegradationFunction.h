//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "REFACTOR_DegradationFunctionBase.h"

/**
 * Predefined rational degradation function
 */
class REFACTOR_RationalDegradationFunction : public REFACTOR_DegradationFunctionBase
{
public:
  static InputParameters validParams();

  REFACTOR_RationalDegradationFunction(const InputParameters & parameters);
};
