//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CrackOpeningDisplacement.h"

registerMooseObject("raccoonApp", CrackOpeningDisplacement);

InputParameters
CrackOpeningDisplacement::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription("This class computes the crack opening displacement.");

  params.addRequiredCoupledVar("phase_field", "The phase-field variable");
  params.addRequiredCoupledVar("levelset", "The levelset variable");
  params.addRequiredCoupledVar(
      "displacements",
      "The displacements appropriate for the simulation geometry and coordinate system");
  params.addParam<RealVectorValue>("normal", "Cheating");

  return params;
}

CrackOpeningDisplacement::CrackOpeningDisplacement(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _wn(declareADProperty<Real>(prependBaseName("crack_opening_displacement"))),
    _disp(adCoupledValues("displacements")),
    _d(adCoupledValue("phase_field")),
    _grad_d(adCoupledGradient("phase_field")),
    _grad_phi(adCoupledGradient("levelset")),
    _normal_provided(isParamValid("normal")),
    _normal(_normal_provided ? getParam<RealVectorValue>("normal") : RealVectorValue(0, 0, 0))
{
  // set unused dimensions to zero
  _disp.resize(3, &_ad_zero);
}

void
CrackOpeningDisplacement::computeQpProperties()
{
  ADRealVectorValue u((*_disp[0])[_qp], (*_disp[1])[_qp], (*_disp[2])[_qp]);

  ADRealVectorValue n;
  if (_normal_provided)
  {
    n = _normal;
    if (_grad_phi[_qp] * _normal < 0)
      n *= -1;
  }
  else if (_grad_phi[_qp].norm() > 1e-6)
    n = _grad_phi[_qp] / _grad_phi[_qp].norm();

  _wn[_qp] = -u * n * _grad_d[_qp].norm();
}
