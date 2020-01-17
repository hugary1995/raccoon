//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CrackVolume.h"

registerMooseObject("raccoonApp", CrackVolume);

template <>
InputParameters
validParams<CrackVolume>()
{
  InputParameters params = validParams<ElementIntegralPostprocessor>();
  params.addRequiredCoupledVar("d", "damage variable");
  params.addRequiredCoupledVar("disp_x", "displacements x");
  params.addRequiredCoupledVar("disp_y", "displacements y");
  return params;
}

CrackVolume::CrackVolume(const InputParameters & parameters)
  : ElementIntegralPostprocessor(parameters),
    _disp_x(coupledValue("disp_x")),
    _disp_y(coupledValue("disp_y")),
    _grad_d(coupledGradient("d"))
{
}

Real
CrackVolume::computeQpIntegral()
{
  return abs(_disp_x[_qp] * _grad_d[_qp](0) + _disp_y[_qp] * _grad_d[_qp](1));
}
