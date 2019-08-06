//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "TimeKernel.h"
#include "Material.h"

// Forward Declarations
class InertialForceFPI;

template <>
InputParameters validParams<InertialForceFPI>();

class InertialForceFPI : public TimeKernel
{
public:
  InertialForceFPI(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

private:
  const VariableValue & _u_old;
  const VariableValue & _vel_old;
  const VariableValue & _accel_old;
  const MaterialProperty<Real> & _density;
  const Real _beta;
  const Real _gamma;
  const Real _dt_master;
};
