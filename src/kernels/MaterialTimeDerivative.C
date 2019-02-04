#include "MaterialTimeDerivative.h"

registerMooseObject("raccoonApp", MaterialTimeDerivative);

template <>
InputParameters
validParams<MaterialTimeDerivative>()
{
  InputParameters params = validParams<TimeDerivative>();
  params.addRequiredParam<MaterialPropertyName>(
      "material_property_name",
      "property name of material property that the time derivative should be multiplied with");

  return params;
}

MaterialTimeDerivative::MaterialTimeDerivative(const InputParameters & parameters)
  : TimeDerivative(parameters), _prop(getMaterialProperty<Real>("material_property_name"))
{
}

Real
MaterialTimeDerivative::computeQpResidual()
{
  return _prop[_qp] * TimeDerivative::computeQpResidual();
}

Real
MaterialTimeDerivative::computeQpJacobian()
{
  return _prop[_qp] * TimeDerivative::computeQpJacobian();
}
