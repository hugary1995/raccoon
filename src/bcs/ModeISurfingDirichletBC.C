//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ModeISurfingDirichletBC.h"

registerMooseObject("raccoonApp", ModeISurfingDirichletBC);

InputParameters
ModeISurfingDirichletBC::validParams()
{
  InputParameters params = NodalBC::validParams();
  params.addParam<RealVectorValue>(
      "initial_crack_tip_position", RealVectorValue(0, 0, 0), "initial crack tip position");
  params.addParam<RealVectorValue>("crack_propagation_velocity",
                                   RealVectorValue(1, 0, 0),
                                   "velocity of the crack tip, crack starts to propagate at t = 1");
  params.addRequiredParam<unsigned int>("component", "0 for x, 1 for y");
  params.addRequiredParam<Real>("Gc", "energy release rate");
  params.addRequiredParam<Real>("E", "Young's modulus");
  params.addRequiredParam<Real>("nu", "Poisson's ratio");
  return params;
}

ModeISurfingDirichletBC::ModeISurfingDirichletBC(const InputParameters & parameters)
  : NodalBC(parameters),
    _c(getParam<RealVectorValue>("initial_crack_tip_position")),
    _v(getParam<RealVectorValue>("crack_propagation_velocity")),
    _component(getParam<unsigned int>("component")),
    _Gc(getParam<Real>("Gc")),
    _E(getParam<Real>("E")),
    _nu(getParam<Real>("nu")),
    _K(3 - 4 * _nu),
    _mu(_E / 2 / (1 + _nu))
{
}

Real
ModeISurfingDirichletBC::computeQpResidual()
{
  RealVectorValue c = _c;
  if (_t > 1)
    c += _v * (_t - 1);
  Real x = (*_current_node)(0) - c(0);
  Real y = (*_current_node)(1) - c(1);
  Real theta = std::atan2(y, x);
  Real r = std::sqrt(x * x + y * y);
  Real K1 = std::sqrt(_E * _Gc / (1 - _nu * _nu));
  K1 *= _t < 1 ? _t : 1;

  Real u = K1 / 2 / _mu * std::sqrt(r / 2 / M_PI) * (_K - std::cos(theta));
  if (_component == 0)
    u *= std::cos(theta / 2);
  if (_component == 1)
    u *= std::sin(theta / 2);
  return _u[_qp] - u;
}
