//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CrackOpeningDisplacementApproximation.h"

registerMooseObject("raccoonApp", CrackOpeningDisplacementApproximation);

InputParameters
CrackOpeningDisplacementApproximation::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription("Approximate the crack opening displacement using the phase field "
                             "variable and the local strain");
  params.addParam<MaterialPropertyName>(
      "crack_opening_displacement", "wn", "Name of the crack opening displacement");
  params.addRequiredCoupledVar(
      "phase_field", "The phase field variable used to approximate the crack surface normal.");
  return params;
}

CrackOpeningDisplacementApproximation::CrackOpeningDisplacementApproximation(
    const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _wn(declareADProperty<Real>(prependBaseName("crack_opening_displacement", true))),
    _grad_d(adCoupledGradient("phase_field")),
    _strain_old(getMaterialPropertyOldByName<RankTwoTensor>(prependBaseName("total_strain")))
{
}

void
CrackOpeningDisplacementApproximation::computeQpProperties()
{
  // Use the phase field gradient to approximate the crack surface normal
  const Real eps = 1e-15;
  ADRealVectorValue n;
  if (_grad_d[_qp].norm() > eps)
    n = _grad_d[_qp] / _grad_d[_qp].norm();

  // Get the characteristic element size
  Real hel = (_current_elem->hmin() + _current_elem->hmax()) / 2;

  _wn[_qp] = hel * (_strain_old[_qp] * n) * n;
}
