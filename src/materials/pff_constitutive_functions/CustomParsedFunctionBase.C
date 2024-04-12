//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CustomParsedFunctionBase.h"

InputParameters
CustomParsedFunctionBase::validParams()
{
  InputParameters params = DerivativeParsedMaterialHelperTempl<true>::validParams();
  params.addRequiredParam<std::string>("expression", "Function to parse");
  params.addParam<std::vector<std::string>>("parameter_names",
                                            std::vector<std::string>(),
                                            "Vector of parameters used in the parsed function");
  params.addParam<std::vector<std::string>>(
      "parameter_values", std::vector<std::string>(), "Vector of values for the parameters");
  params.addParam<std::vector<std::string>>(
      "material_property_names",
      std::vector<std::string>(),
      "Vector of material properties used in the degradation function");
  return params;
}

CustomParsedFunctionBase::CustomParsedFunctionBase(const InputParameters & parameters)
  : DerivativeParsedMaterialHelperTempl<true>(parameters, VariableNameMappingMode::USE_MOOSE_NAMES),
    _function(getParam<std::string>("expression")),
    _param_names(getParam<std::vector<std::string>>("parameter_names")),
    _param_values(getParam<std::vector<std::string>>("parameter_values")),
    _mat_props(getParam<std::vector<std::string>>("material_property_names"))
{
  // Build function, take derivatives, optimize
  functionParse(_function,
                _param_names,
                _param_values,
                _mat_props,
                std::vector<std::string>(),
                std::vector<Real>());
}
