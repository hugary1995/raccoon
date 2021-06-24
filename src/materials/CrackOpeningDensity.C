//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CrackOpeningDensity.h"

registerMooseObject("raccoonApp", CrackOpeningDensity);

InputParameters
CrackOpeningDensity::validParams()
{
  InputParameters params = Material::validParams();
  params.addRequiredCoupledVar(
      "displacements",
      "The displacements appropriate for the simulation geometry and coordinate system");
  params.addRequiredCoupledVar("phase_field", "The phase-field variable");

  return params;
}

CrackOpeningDensity::CrackOpeningDensity(const InputParameters & parameters)
  : ADMaterial(parameters),
    _ndisp(coupledComponents("displacements")),
    _disp(3),
    _grad_d(adCoupledGradient("phase_field")),
    _gap(declareADProperty<Real>("gap"))
{
  // fetch coupled variables and gradients (as stateful properties if necessary)
  for (unsigned int i = 0; i < _ndisp; ++i)
    _disp[i] = &adCoupledValue("displacements", i);
  // set unused dimensions to zero
  for (unsigned i = _ndisp; i < 3; ++i)
    _disp[i] = &adZeroValue();
}

void
CrackOpeningDensity::computeQpProperties()
{
  ADRealVectorValue u((*_disp[0])[_qp], (*_disp[1])[_qp], (*_disp[2])[_qp]);
  _gap[_qp] = -u * _grad_d[_qp];
}
