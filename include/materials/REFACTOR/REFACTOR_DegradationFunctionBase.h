//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "REFACTOR_CustomParsedFunctionBase.h"

/**
 * Class to evaluate the degradation function and automatically provide all derivatives.
 */
class REFACTOR_DegradationFunctionBase : public REFACTOR_CustomParsedFunctionBase
{
public:
  static InputParameters validParams();

  REFACTOR_DegradationFunctionBase(const InputParameters & parameters);
};
