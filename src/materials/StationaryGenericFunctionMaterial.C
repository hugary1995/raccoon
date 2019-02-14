#include "StationaryGenericFunctionMaterial.h"
#include "Function.h"

registerMooseObject("raccoonApp", StationaryGenericFunctionMaterial);

template <>
InputParameters
validParams<StationaryGenericFunctionMaterial>()
{
  InputParameters params = validParams<Material>();
  params.addParam<std::vector<std::string>>("prop_names",
                                            "The names of the properties this material will have");
  params.addParam<std::vector<FunctionName>>("prop_values",
                                             "The corresponding names of the "
                                             "functions that are going to provide "
                                             "the values for the variables");
  return params;
}

StationaryGenericFunctionMaterial::StationaryGenericFunctionMaterial(
    const InputParameters & parameters)
  : Material(parameters),
    _prop_names(getParam<std::vector<std::string>>("prop_names")),
    _prop_values(getParam<std::vector<FunctionName>>("prop_values"))
{
  unsigned int num_names = _prop_names.size();
  unsigned int num_values = _prop_values.size();

  if (num_names != num_values)
    mooseError("Number of prop_names much match the number of prop_values for a "
               "StationaryGenericFunctionMaterial!");

  _num_props = num_names;
  _properties.resize(num_names);
  _properties_old.resize(num_names);
  _functions.resize(num_names);

  for (unsigned int i = 0; i < _num_props; i++)
  {
    _properties[i] = &declareProperty<Real>(_prop_names[i]);
    _properties_old[i] = &getMaterialPropertyOldByName<Real>(_prop_names[i]);
    _functions[i] = &getFunctionByName(_prop_values[i]);
  }
}

void
StationaryGenericFunctionMaterial::initQpStatefulProperties()
{
  for (unsigned int i = 0; i < _num_props; i++)
    (*_properties[i])[_qp] = (*_functions[i]).value(_t, _q_point[_qp]);
}

void
StationaryGenericFunctionMaterial::computeQpProperties()
{
  for (unsigned int i = 0; i < _num_props; i++)
    (*_properties[i])[_qp] = (*_properties_old[i])[_qp];
}
