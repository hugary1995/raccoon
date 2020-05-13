//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADCoefMatDiffusion.h"
#include "Assembly.h"

registerADMooseObject("raccoonApp", ADCoefMatDiffusion);

InputParameters
ADCoefMatDiffusion::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addClassDescription(
      "Reaction term optionally multiplied with a coefficient and a material property");
  params.addParam<Real>("coefficient", 1.0, "Coefficient of the term");
  params.addParam<MaterialPropertyName>("material_property_name",
                                        "name of the material property to provide the multiplier");
  return params;
}

ADCoefMatDiffusion::ADCoefMatDiffusion(const InputParameters & parameters)
  : ADKernel(parameters),
    _coef(getParam<Real>("coefficient")),
    _prop_names(getParam<std::vector<MaterialPropertyName>>("prop_names")),
    _num_props(_prop_names.size()),
    _coord_sys(_assembly.coordSystem())
{
  _props.resize(_num_props);
  for (unsigned int i = 0; i < _num_props; i++)
    _props[i] = &getADMaterialProperty<Real>(_prop_names[i]);
}

ADReal
ADCoefMatDiffusion::computeQpResidual()
{
  ADReal factor = _coef;
  for (auto prop : _props)
    factor *= (*prop)[_qp];

  ADReal value = _grad_test[_i][_qp] * _grad_u[_qp];
  if (_coord_sys == Moose::COORD_RZ)
    value -= _test[_i][_qp] / _ad_q_point[_qp](0) * _grad_u[_qp](0);

  return factor * value;
}
