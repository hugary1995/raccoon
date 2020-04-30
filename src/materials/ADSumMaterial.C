//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADSumMaterial.h"

registerADMooseObject("raccoonApp", ADSumMaterial);

InputParameters
ADSumMaterial::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params.addRequiredParam<MaterialPropertyName>(
      "sum_prop_name", "The name of the property that holds the summation");
  params.addRequiredParam<std::vector<MaterialPropertyName>>(
      "prop_names", "The names of the properties to be summed");

  return params;
}

ADSumMaterial::ADSumMaterial(const InputParameters & parameters)
  : ADMaterial(parameters),
    _sum(declareADProperty<Real>(getParam<MaterialPropertyName>("sum_prop_name"))),
    _prop_names(getParam<std::vector<MaterialPropertyName>>("prop_names")),
    _num_props(_prop_names.size())
{
  _props.resize(_num_props);
  for (unsigned int i = 0; i < _num_props; i++)
    _props[i] = &getADMaterialProperty<Real>(_prop_names[i]);
}

void
ADSumMaterial::initQpStatefulProperties()
{
  _sum[_qp] = 0.0;
}

void
ADSumMaterial::computeQpProperties()
{
  _sum[_qp] = 0.0;
  for (unsigned int i = 0; i < _num_props; i++)
    _sum[_qp] += (*_props[i])[_qp];
}
