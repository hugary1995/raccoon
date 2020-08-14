//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PFFCrackOpening.h"

registerMooseObject("raccoonApp", PFFCrackOpening);

InputParameters
PFFCrackOpening::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("Compute crack opening displacement (COD)");
  params.addRequiredCoupledVar("displacements",
                               "The string of displacements suitable for the problem statement");
  params.addRequiredCoupledVar("d", "Order parameter for damage.");
  return params;
}

PFFCrackOpening::PFFCrackOpening(const InputParameters & parameters)
  : AuxKernel(parameters),
    _ndisp(coupledComponents("displacements")),
    _disp(_ndisp),
    _grad_c(coupledGradient("d"))
{
  for (unsigned int i = 0; i < _ndisp; ++i)
    _disp[i] = &coupledValue("displacements", i);
}

Real
PFFCrackOpening::computeValue()
{
  RealVectorValue disp((*_disp[0])[_qp], (*_disp[1])[_qp], 0.0);
  if (_ndisp == 3)
    disp(2) = (*_disp[2])[_qp];

  return disp * _grad_c[_qp];
}
