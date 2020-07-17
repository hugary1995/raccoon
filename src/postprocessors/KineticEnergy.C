//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "KineticEnergy.h"

registerMooseObject("raccoonApp", KineticEnergy);

InputParameters
KineticEnergy::validParams()
{
  InputParameters params = ElementIntegralPostprocessor::validParams();
  params.addClassDescription("computes the total fracture energy of the form $\\int_\\body "
                             "0.5\\rho \\dot{u} \\cdot \\dot{u} \\diff{V}$.");
  params.addRequiredCoupledVar("displacements",
                               "The string of displacements suitable for the problem statement");
  params.addParam<MaterialPropertyName>(
      "density", "density", "Name of material property containing density");
  return params;
}

KineticEnergy::KineticEnergy(const InputParameters & parameters)
  : ElementIntegralPostprocessor(parameters),
    _rho(getMaterialProperty<Real>("density")),
    _ndisp(coupledComponents("displacements")),
    _vel_var(_ndisp)
{
  for (unsigned int i = 0; i < _ndisp; ++i)
    _vel_var[i] = &adCoupledDot("displacements", i);
}

Real
KineticEnergy::computeQpIntegral()
{
  Real ans = 0;
  for (unsigned i = 0; i < _ndisp; ++i)
  {
    ans += ((*_vel_var[i])[_qp] * (*_vel_var[i])[_qp]).value();
  }
  return 0.5 * _rho[_qp] * ans;
}
