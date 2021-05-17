//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "KineticEnergy.h"

registerMooseObject("raccoonApp", KineticEnergy);

InputParameters
KineticEnergy::validParams()
{
  InputParameters params = ElementIntegralPostprocessor::validParams();
  params.addClassDescription(
      "This class computes the total kinetic energy of the form $\\int_\\body "
      "0.5\\rho \\dot{u} \\cdot \\dot{u} \\diff{V}$.");
  params.addRequiredCoupledVar("displacements",
                               "The string of displacements suitable for the problem statement");
  params.addParam<MaterialPropertyName>(
      "density", "density", "Name of material property containing density");
  return params;
}

KineticEnergy::KineticEnergy(const InputParameters & parameters)
  : ElementIntegralPostprocessor(parameters),
    _rho(getADMaterialProperty<Real>("density")),
    _ndisp(coupledComponents("displacements")),
    _u_dots(coupledDots("displacements"))
{
  for (unsigned int i = _ndisp; i < 3; ++i)
    _u_dots.push_back(&_zero);
}

Real
KineticEnergy::computeQpIntegral()
{
  RealVectorValue u_dot((*_u_dots[0])[_qp], (*_u_dots[1])[_qp], (*_u_dots[2])[_qp]);
  return 0.5 * raw_value(_rho[_qp]) * u_dot * u_dot;
}
