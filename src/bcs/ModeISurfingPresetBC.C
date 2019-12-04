//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ModeISurfingPresetBC.h"
#include <math.h>

registerMooseObject("raccoonApp", ModeISurfingPresetBC);

template <>
InputParameters
validParams<ModeISurfingPresetBC>()
{
  InputParameters params = validParams<PresetNodalBC>();
  params.addRequiredCoupledVar("crack_tip_x",
                               "scalar variable describing the x coordinate of the crack tip");
  params.addRequiredCoupledVar("crack_tip_y",
                               "scalar variable describing the y coordinate of the crack tip");
  params.addRequiredParam<unsigned int>("component", "0 for x, 1 for y");
  params.addRequiredParam<Real>("Gc", "energy release rate");
  params.addRequiredParam<Real>("E", "Young's modulus");
  params.addRequiredParam<Real>("nu", "Poisson's ratio");
  return params;
}

ModeISurfingPresetBC::ModeISurfingPresetBC(const InputParameters & parameters)
  : PresetNodalBC(parameters),
    _xc(coupledScalarValue("crack_tip_x")),
    _yc(coupledScalarValue("crack_tip_y")),
    _component(getParam<unsigned int>("component")),
    _Gc(getParam<Real>("Gc")),
    _E(getParam<Real>("E")),
    _nu(getParam<Real>("nu"))
{
}

Real
ModeISurfingPresetBC::computeQpValue()
{
  Real x = (*_current_node)(0) - _xc[0];
  Real y = (*_current_node)(1) - _yc[0];
  Real theta = std::atan2(y, x);
  Real r = std::sqrt(x * x + y * y);

  Real u = std::sqrt(r * _Gc / 2.0 / M_PI / _E) * (3.0 - _nu - (1.0 + _nu) * std::cos(theta));
  u *= _component == 0 ? std::cos(theta / 2.0) : std::sin(theta / 2.0);

  return u;
}
