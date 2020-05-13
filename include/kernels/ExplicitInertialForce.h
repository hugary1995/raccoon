//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Kernel.h"

class ExplicitInertialForce : public Kernel
{
public:
  static InputParameters validParams();

  ExplicitInertialForce(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

private:
  const MaterialProperty<Real> & _density;
  const VariableValue & _u_old;
  const VariableValue & _u_older;
};
