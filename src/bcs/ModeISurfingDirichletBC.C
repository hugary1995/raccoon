//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ModeISurfingDirichletBC.h"

registerMooseObject("raccoonApp", ModeISurfingDirichletBC);

InputParameters
ModeISurfingDirichletBC::validParams()
{
  InputParameters params = NodalBC::validParams();
  params.addClassDescription(
      "This class applies the Dirichlet BC conforming with the analytical solution of a Mode-I "
      "crack. The crack is assumed to be emanating from the origin. For $t \\in [0, 1]$ the BC "
      "ramps up linearly to match the initial crack tip position, and for $t \\in [1, \\infty)$, "
      "the crack tip advances to the right with a velocity of v");
  params.addParam<Point>(
      "initial_crack_tip_position", RealVectorValue(0, 0, 0), "Initial crack tip position");
  params.addParam<RealVectorValue>("crack_propagation_velocity",
                                   RealVectorValue(1, 0, 0),
                                   "Velocity of the crack tip, crack starts to propagate at t = 1");
  params.addRequiredParam<unsigned int>("component", "0 for x, 1 for y");
  params.addRequiredParam<Real>("Gc", "Fracture toughness");
  params.addRequiredParam<Real>("K", "Bulk modulus");
  params.addRequiredParam<Real>("G", "Shear modulus");
  return params;
}

ModeISurfingDirichletBC::ModeISurfingDirichletBC(const InputParameters & parameters)
  : NodalBC(parameters),
    _c(getParam<Point>("initial_crack_tip_position")),
    _v(getParam<RealVectorValue>("crack_propagation_velocity")),
    _component(getParam<unsigned int>("component")),
    _Gc(getParam<Real>("Gc")),
    _K(getParam<Real>("K")),
    _G(getParam<Real>("G"))
{
}

Real
ModeISurfingDirichletBC::computeQpResidual()
{
  Real E = 9 * _K * _G / (3 * _K + _G);
  Real nu = (3 * _K - 2 * _G) / 2 / (3 * _K + _G);
  Real Kolosov = (3 - 4 * nu);

  Point c = _c;
  if (_t > 1)
    c += _v * (_t - 1);
  Real x = (*_current_node)(0) - c(0);
  Real y = (*_current_node)(1) - c(1);
  Real theta = std::atan2(y, x);
  Real r = std::sqrt(x * x + y * y);
  Real K1 = std::sqrt(E * _Gc / (1 - nu * nu));
  K1 *= _t < 1 ? _t : 1;

  Real u = K1 / 2 / _G * std::sqrt(r / 2 / M_PI) * (Kolosov - std::cos(theta));
  if (_component == 0)
    u *= std::cos(theta / 2);
  if (_component == 1)
    u *= std::sin(theta / 2);
  return _u[_qp] - u;
}
