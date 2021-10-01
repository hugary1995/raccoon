//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "InputParameters.h"

class BaseNameInterface
{
public:
  static InputParameters validParams();

  BaseNameInterface(const InputParameters & params);
  virtual ~BaseNameInterface() = default;

  // Simply prepend the base name to a given string if get_property_name = false. If
  // get_property_name = true, prepend the base name to the retrieved MaterialPropertyName.
  const MaterialPropertyName prependBaseName(const MaterialPropertyName s,
                                             const bool get_property_name = false) const;

  // Prepend base name to each of the vector component
  const std::vector<MaterialPropertyName>
  prependBaseName(const std::vector<MaterialPropertyName> & s) const;

protected:
  /// The parameters
  const InputParameters & _params;

  /// The base name
  const std::string _base_name;
};
