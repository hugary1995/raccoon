//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ExplicitInertialForce.h"

registerMooseObject("raccoonApp", ExplicitInertialForce);

InputParameters
ExplicitInertialForce::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addClassDescription("Calculates the residual for the interial force (M*accel) and the "
                             "contribution of mass dependent Rayleigh damping and HHT time "
                             "integration scheme $eta*M*((1+alpha)vel-alpha*vel_old)$");
  params.set<bool>("use_displaced_mesh") = true;
  return params;
}

ExplicitInertialForce::ExplicitInertialForce(const InputParameters & parameters)
  : Kernel(parameters),
    _density(getMaterialProperty<Real>("density")),
    _u_old(valueOld()),
    _u_older(valueOlder())
{
}

Real
ExplicitInertialForce::computeQpResidual()
{
  Real accel = 1. / (_dt * _dt) * (_u[_qp] - _u_old[_qp] * 2.0 + _u_older[_qp]);

  return _test[_i][_qp] * _density[_qp] * accel;
}

Real
ExplicitInertialForce::computeQpJacobian()
{
  return _test[_i][_qp] * _density[_qp] / (_dt * _dt) * _phi[_j][_qp];
}
