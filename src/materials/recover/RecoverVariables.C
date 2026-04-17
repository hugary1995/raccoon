//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "RecoverVariables.h"

registerMooseObject("MooseApp", RecoverVariables);

InputParameters
RecoverVariables::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription(
      "Declares material properties based on names and values prescribed by input parameters.");
  params.addRequiredParam<std::vector<std::string>>(
      "prop_names", "The names of the properties this material will have");
  params.addRequiredParam<std::vector<Real>>("prop_values",
                                             "The values associated with the named properties");
  params.set<MooseEnum>("constant_on") = "SUBDOMAIN";
  params.declareControllable("prop_values");
  return params;
}

RecoverVariables::RecoverVariables(const InputParameters & parameters)
  : Material(parameters),
    _real_prop_names(getParam<std::vector<std::string>>("prop_names")),
    _real_prop_values(getParam<std::vector<Real>>("prop_values"))
{
  unsigned int num_names = _real_prop_names.size();
  unsigned int num_values = _real_prop_values.size();

  if (num_names != num_values)
    mooseError("Number of prop_names must match the number of prop_values for RecoverVariables!");

  _num_props = num_names;

  _properties.resize(num_names);
  _get_properties.resize(num_names);
  for (unsigned int i = 0; i < _num_props; i++)
    _properties[i] = &declareProperty<Real>(_real_prop_names[i] + "_sol");

  for (unsigned int i = 0; i < _num_props; i++)
    _get_properties[i] = &getADMaterialProperty<Real>(_real_prop_names[i]);
}

void
RecoverVariables::initQpStatefulProperties()
{
  computeQpProperties();
}

void
RecoverVariables::computeQpProperties()
{
  for (unsigned int i = 0; i < _num_props; i++)
    (*_properties[i])[_qp] = _real_prop_values[i];
}
