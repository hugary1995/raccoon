//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CoefMatVectorSource.h"

registerADMooseObject("raccoonApp", CoefMatVectorSource);

InputParameters
CoefMatVectorSource::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  params.addClassDescription(
      "Reaction term optionally multiplied with a coefficient and a material property");
  params.addParam<Real>("coefficient", 1.0, "Coefficient of the term");
  params.addParam<std::vector<MaterialPropertyName>>(
      "prop_names", "names of the material properties to provide the multiplier");
  params.addParam<unsigned int>("component", "0, 1, or 2");
  return params;
}

CoefMatVectorSource::CoefMatVectorSource(const InputParameters & parameters)
  : ADKernelValue(parameters),
    _coef(getParam<Real>("coefficient")),
    _prop_names(getParam<std::vector<MaterialPropertyName>>("prop_names")),
    _num_props(_prop_names.size()),
    _component(getParam<unsigned int>("component"))
{
  _props.resize(_num_props);
  for (unsigned int i = 0; i < _num_props; i++)
    _props[i] = &getMaterialProperty<RealVectorValue>(_prop_names[i]);
}

ADReal
CoefMatVectorSource::precomputeQpResidual()
{
  ADReal factor = 1.0;
  for (auto prop : _props)
    factor *= (*prop)[_qp](_component);

  return factor * _coef;
}
