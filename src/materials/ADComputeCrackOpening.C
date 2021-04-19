//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADComputeCrackOpening.h"

registerMooseObject("raccoonApp", ADComputeCrackOpening);

InputParameters
ADComputeCrackOpening::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription(
      "compute crack opening using the damage phase field variable and strain elemental tensor");
  params.addParam<MaterialPropertyName>(
      "crack_opening_name", "crack_opening", "Name of the crack opening variable");
  params.addRequiredCoupledVar("damage", "damage field");
  return params;
}

ADComputeCrackOpening::ADComputeCrackOpening(const InputParameters & parameters)
  : Material(parameters),
    _wn(declareADProperty<Real>(getParam<MaterialPropertyName>("crack_opening_name"))),
    _d(adCoupledValue("damage")),
    _grad_d(adCoupledGradient("damage")),
    _strain(getADMaterialPropertyByName<RankTwoTensor>("total_strain")),
    _strain_old(getMaterialPropertyOldByName<RankTwoTensor>("total_strain"))
{
}

void
ADComputeCrackOpening::computeQpProperties()
{
  const Real eps = 1e-15;
  ADRealVectorValue n;
  if (_grad_d[_qp].norm() > eps)
    n = _grad_d[_qp] / _grad_d[_qp].norm();
  _wn[_qp] = 0.5 * (_current_elem->hmin() + _current_elem->hmax()) * (_strain_old[_qp] * n) * n;
}
