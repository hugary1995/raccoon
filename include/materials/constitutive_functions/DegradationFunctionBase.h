//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "CustomParsedFunctionBase.h"

/**
 * Class to evaluate the degradation function and automatically provide all derivatives.
 */
class DegradationFunctionBase : public CustomParsedFunctionBase
{
public:
  static InputParameters validParams();

  DegradationFunctionBase(const InputParameters & parameters);
};
