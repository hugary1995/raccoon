//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "BaseNameInterface.h"

InputParameters
BaseNameInterface::validParams()
{
  InputParameters params = emptyInputParameters();
  params.addParam<std::string>("base_name",
                               "Optional parameter that allows the user to define "
                               "multiple mechanics material systems on the same "
                               "block, i.e. for multiple phases");
  return params;
}

BaseNameInterface::BaseNameInterface(const InputParameters & params)
  : _bi_params(params),
    _base_name(params.isParamValid("base_name") ? params.get<std::string>("base_name") + "_" : "")
{
}

const MaterialPropertyName
BaseNameInterface::prependBaseName(const MaterialPropertyName s, const bool get_property_name) const
{
  if (get_property_name)
    return _base_name + _bi_params.get<MaterialPropertyName>(s);
  return _base_name + s;
}

const std::vector<MaterialPropertyName>
BaseNameInterface::prependBaseName(const std::vector<MaterialPropertyName> & s) const
{
  std::vector<MaterialPropertyName> s_copy;
  for (const auto & si : s)
    s_copy.push_back(prependBaseName(si));
  return s_copy;
}
