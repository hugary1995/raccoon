//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#define _USE_MATH_DEFINES
#include <cmath>
#include "DiracSource.h"
#include "Executioner.h"

registerMooseObject("raccoonApp", DiracSource);

InputParameters
DiracSource::validParams()
{
  InputParameters params = DiracKernel::validParams();
  params.addRequiredParam<Point>("point", "The x,y,z coordinates of the point");
  params.addRequiredParam<int>("dim", "The dimension of the problem");
  params.addRequiredParam<Real>("fL", "some coefficient");
  params.addRequiredParam<Real>("t1", "some coefficient");
  params.addRequiredParam<Real>("tRT", "some coefficient");
  params.addRequiredParam<Real>("tL", "some coefficient");
  params.addRequiredParam<Real>("tP", "some coefficient");
  params.addRequiredParam<Real>("p0", "some coefficient");
  params.addRequiredParam<Real>("d1", "some coefficient");
  params.addRequiredParam<Real>("upcoeff", "some coefficient");
  params.addRequiredParam<Real>("downcoeff", "some coefficient");
  params.addRequiredParam<Real>("rho", "some coefficient");
  params.addRequiredParam<Real>("num_shots", "number of shots");
  return params;
}

DiracSource::DiracSource(const InputParameters & parameters)
  : DiracKernel(parameters),
    _point(getParam<Point>("point")),
    _dim(getParam<int>("dim")),
    _fL(getParam<Real>("fL")),
    _t1(getParam<Real>("t1")),
    _tRT(getParam<Real>("tRT")),
    _tL(getParam<Real>("tL")),
    _tP(getParam<Real>("tP")),
    _p0(getParam<Real>("p0")),
    _d1(getParam<Real>("d1")),
    _upcoeff(getParam<Real>("upcoeff")),
    _downcoeff(getParam<Real>("downcoeff")),
    _rho(getParam<Real>("rho")),
    _num_shots(getParam<Real>("num_shots"))
{
}

void
DiracSource::addPoints()
{
  // Add a point from the input file
  addPoint(_point);
}

Real
DiracSource::computeQpResidual()
{
  const Real _end_t = _app.getExecutioner()->getParam<Real>("end_time");
  Real _shot_duration = _end_t / _num_shots;
  int _current_shot = std::floor(_t / _shot_duration);
  Real _shot_t = _t - _current_shot * _shot_duration;

  // Yingjie's paper
  Real _pressure_source =
      1 / _tP * 4 * M_PI / _rho * _upcoeff / _downcoeff * _p0 * _d1 *
      (std::max((1 + tanh((_shot_t - _t1) / _tRT)) * std::exp(-(_shot_t - _t1) / _tL) *
                    cos(2 * M_PI * _fL * (_shot_t - _t1) + M_PI / 3),
                0.0));

  // source pressure times test function is returned
  return -_test[_i][_qp] * _pressure_source;
}
