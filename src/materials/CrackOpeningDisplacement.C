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
  params.addRequiredCoupledVar(
      "displacements",
      "The displacements appropriate for the simulation geometry and coordinate system");

  return params;
}

CrackOpeningDisplacement::CrackOpeningDisplacement(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _wn(declareADProperty<Real>(prependBaseName("crack_opening_displacement"))),
    _disp(adCoupledValues("displacements")),
    _d(adCoupledValue("phase_field")),
    _grad_d(adCoupledGradient("phase_field"))
{
  // set unused dimensions to zero
  _disp.resize(3, &_ad_zero);
}

void
CrackOpeningDisplacement::computeQpProperties()
{
  if (_d[_qp] > 1e-6 && _grad_d[_qp].norm() > 1e-6)
  {
    ADRealVectorValue u((*_disp[0])[_qp], (*_disp[1])[_qp], (*_disp[2])[_qp]);
    ADRealVectorValue n = _grad_d[_qp] / _grad_d[_qp].norm();
    _wn[_qp] = -u * n * _grad_d[_qp].norm();
  }
  else
    _wn[_qp] = 0;
}
