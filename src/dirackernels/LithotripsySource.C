//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#define _USE_MATH_DEFINES
#include <cmath>
#include "LithotripsySource.h"

registerMooseObject("raccoonApp", LithotripsySource);

InputParameters
LithotripsySource::validParams()
{
  InputParameters params = DiracKernel::validParams();
  params.addClassDescription("adds a point source in the domain according to the experimental "
                             "paramters of a shockwave lithotripsy test.");
  params.addRequiredParam<Point>("point", "The x,y,z coordinates of the point");
  params.addRequiredParam<Real>("fL", "some coefficient");
  params.addRequiredParam<Real>("t1", "some coefficient");
  params.addRequiredParam<Real>("tRT", "some coefficient");
  params.addRequiredParam<Real>("tL", "some coefficient");
  params.addRequiredParam<Real>("tP", "some coefficient");
  params.addRequiredParam<Real>("p0", "some coefficient");
  params.addRequiredParam<Real>("d1", "some coefficient");
  params.addRequiredParam<Real>("upcoeff", "some coefficient");
  params.addRequiredParam<Real>("downcoeff", "some coefficient");
  params.addRequiredParam<MaterialPropertyName>("diffusivity", "diffusivity");
  return params;
}

LithotripsySource::LithotripsySource(const InputParameters & parameters)
  : DiracKernel(parameters),
    _point(getParam<Point>("point")),
    _fL(getParam<Real>("fL")),
    _t1(getParam<Real>("t1")),
    _tRT(getParam<Real>("tRT")),
    _tL(getParam<Real>("tL")),
    _tP(getParam<Real>("tP")),
    _p0(getParam<Real>("p0")),
    _d1(getParam<Real>("d1")),
    _upcoeff(getParam<Real>("upcoeff")),
    _downcoeff(getParam<Real>("downcoeff")),
    _diff(getMaterialProperty<Real>("diffusivity"))
{
}

void
LithotripsySource::addPoints()
{
  // Add a point from the input file
  addPoint(_point);
}

Real
LithotripsySource::computeQpResidual()
{
  // This is negative because it's a pressure function
  return -4 * M_PI * _diff[_qp] * _upcoeff / _downcoeff * _p0 * _d1 *
         (std::max((1 + tanh((_t - _t1) / _tRT) * std::exp(-(_t - _t1) / _tL) *
                            cos(2 * M_PI * _fL * (_t - _t1) + M_PI / 3)),
                   0.0));
}
