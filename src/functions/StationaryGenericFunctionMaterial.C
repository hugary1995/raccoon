//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "StationaryGenericFunctionMaterial.h"

registerMooseObject("raccoonApp", StationaryGenericFunctionMaterial);

InputParameters
StationaryGenericFunctionMaterial::validParams()
{
  InputParameters params = GenericFunctionMaterial::validParams();
  return params;
}

StationaryGenericFunctionMaterial::StationaryGenericFunctionMaterial(
    const InputParameters & parameters)
  : GenericFunctionMaterial(parameters)
{
  _properties_old.resize(_num_props);
  for (unsigned int i = 0; i < _num_props; i++)
    _properties_old[i] = &getMaterialPropertyOldByName<Real>(_prop_names[i]);
}

void
StationaryGenericFunctionMaterial::computeQpProperties()
{
  for (unsigned int i = 0; i < _num_props; i++)
    (*_properties[i])[_qp] = (*_properties_old[i])[_qp];
}
