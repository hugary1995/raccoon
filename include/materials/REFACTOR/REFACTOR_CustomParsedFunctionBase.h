//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

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
