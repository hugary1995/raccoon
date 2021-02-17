//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "DerivativeParsedMaterialHelper.h"

/**
 * Class to evaluate the custom function and automatically provide all derivatives.
 */
class REFACTOR_CustomParsedFunctionBase : public DerivativeParsedMaterialHelperTempl<true>
{
public:
  static InputParameters validParams();

  REFACTOR_CustomParsedFunctionBase(const InputParameters & parameters);

  usingDerivativeParsedMaterialHelperMembers(true);

private:
  const std::string _function;
  const std::vector<std::string> _param_names;
  const std::vector<std::string> _param_values;
  const std::vector<std::string> _mat_props;
};
