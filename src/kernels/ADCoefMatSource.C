//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADCoefMatSource.h"

registerMooseObject("raccoonApp", ADCoefMatSource);

InputParameters
ADCoefMatSource::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  params.addClassDescription(
      "Source term defined by the product of a coefficient and material properties");
  params.addParam<Real>("coefficient", 1.0, "Coefficient of the term");
  params.addParam<std::vector<MaterialPropertyName>>(
      "prop_names", {}, "names of the material properties to provide the multiplier");
  return params;
}

ADCoefMatSource::ADCoefMatSource(const InputParameters & parameters)
  : ADKernelValue(parameters),
    _coef(getParam<Real>("coefficient")),
    _prop_names(getParam<std::vector<MaterialPropertyName>>("prop_names")),
    _num_props(_prop_names.size())
{
  _props.resize(_num_props);
  for (unsigned int i = 0; i < _num_props; i++)
    _props[i] = &getADMaterialProperty<Real>(_prop_names[i]);
}

ADReal
ADCoefMatSource::precomputeQpResidual()
{
  ADReal factor = 1.0;
  for (auto prop : _props)
    factor *= (*prop)[_qp];

  return factor * _coef;
}
