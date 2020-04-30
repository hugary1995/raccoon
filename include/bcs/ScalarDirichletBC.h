//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "NodalBC.h"

/**
 * Defines a boundary condition that forces the value to be a user specified
 * scalar at the boundary.
 */
class ScalarDirichletBC : public NodalBC
{
public:
  static InputParameters validParams();

  ScalarDirichletBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;

  /// The scalar value
  const VariableValue & _scalar_val;
};
