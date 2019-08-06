//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ScalarBodyForce.h"

registerMooseObject("raccoonApp", ScalarBodyForce);

template <>
InputParameters
validParams<ScalarBodyForce>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Demonstrates the multiple ways that scalar values can be introduced "
                             "into kernels, e.g. (controllable) constants, functions, and "
                             "postprocessors. Implements the weak form $(\\psi_i, -f)$.");
  params.addRequiredCoupledVar("scalar", "The scalar variable coupled in");
  return params;
}

ScalarBodyForce::ScalarBodyForce(const InputParameters & parameters)
  : Kernel(parameters), _scalar(coupledScalarValue("scalar"))
{
}

Real
ScalarBodyForce::computeQpResidual()
{
  return -_test[_i][_qp] * _scalar[0];
}
