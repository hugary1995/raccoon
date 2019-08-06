//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "InertialForceFPI.h"
#include "SubProblem.h"

registerMooseObject("raccoonApp", InertialForceFPI);

template <>
InputParameters
validParams<InertialForceFPI>()
{
  InputParameters params = validParams<TimeKernel>();
  params.addClassDescription("Calculates the residual for the interial force "
                             "($M \\cdot acceleration$) and the contribution of mass"
                             " dependent Rayleigh damping and HHT time "
                             " integration scheme ($\\eta \\cdot M \\cdot"
                             " ((1+\\alpha)velq2-\\alpha \\cdot vel-old) $)");
  params.addRequiredCoupledVar("velocity", "velocity variable");
  params.addRequiredCoupledVar("acceleration", "acceleration variable");
  params.addRequiredCoupledVar("displacement_old", "last converged displacement variable");
  params.addRequiredParam<Real>("beta", "beta parameter for Newmark Time integration");
  params.addRequiredParam<Real>("gamma", "gamma parameter for Newmark Time integration");
  params.addParam<MaterialPropertyName>(
      "density", "density", "Name of Material Property that provides the density");
  params.addRequiredParam<Real>("dt_master", "master app time step size");
  return params;
}

InertialForceFPI::InertialForceFPI(const InputParameters & parameters)
  : TimeKernel(parameters),
    _u_old(coupledValue("displacement_old")),
    _vel_old(coupledValue("velocity")),
    _accel_old(coupledValue("acceleration")),
    _density(getMaterialProperty<Real>("density")),
    _beta(getParam<Real>("beta")),
    _gamma(getParam<Real>("gamma")),
    _dt_master(getParam<Real>("dt_master"))
{
}

Real
InertialForceFPI::computeQpResidual()
{
  if (_dt == 0)
    return 0;
  else
  {
    Real accel = 1.0 / _beta *
                 (((_u[_qp] - _u_old[_qp]) / (_dt_master * _dt_master)) -
                  _vel_old[_qp] / _dt_master - _accel_old[_qp] * (0.5 - _beta));
    return _test[_i][_qp] * _density[_qp] * accel;
  }
}

Real
InertialForceFPI::computeQpJacobian()
{
  if (_dt == 0)
    return 0;
  else
    return _test[_i][_qp] * _density[_qp] / (_beta * _dt_master * _dt_master) * _phi[_j][_qp];
}
