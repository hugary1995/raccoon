//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "FractureEnergy.h"

registerMooseObject("raccoonApp", FractureEnergy);

template <>
InputParameters
validParams<FractureEnergy>()
{
  InputParameters params = validParams<ElementIntegralPostprocessor>();
  params.addRequiredCoupledVar("d", "damage variable");
  params.addParam<MaterialPropertyName>(
      "kappa_name", "kappa", "name of the material that holds the interface coefficient");
  params.addParam<MaterialPropertyName>(
      "mobility_name", "mobility", "name of the material that holds the mobility");
  params.addParam<MaterialPropertyName>(
      "local_dissipation_name",
      "w",
      "name of the material that holds the local dissipation function");
  return params;
}

FractureEnergy::FractureEnergy(const InputParameters & parameters)
  : ElementIntegralPostprocessor(parameters),
    _kappa(getMaterialProperty<Real>("kappa_name")),
    _M(getMaterialProperty<Real>("mobility_name")),
    _grad_d(coupledGradient("d")),
    _w(getMaterialProperty<Real>("local_dissipation_name"))
{
}

Real
FractureEnergy::computeQpIntegral()
{
  return _M[_qp] * (_w[_qp] + 0.5 * _kappa[_qp] * _grad_d[_qp] * _grad_d[_qp]);
}
