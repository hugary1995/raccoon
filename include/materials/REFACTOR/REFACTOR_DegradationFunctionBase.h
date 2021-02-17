//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

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
