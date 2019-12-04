//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "NodalBC.h"

// Forward Declarations
class ModeISurfingDirichletBC;

template <>
InputParameters validParams<ModeISurfingDirichletBC>();

/**
 * Defines a boundary condition that forces the value to be a user specified
 * scalar at the boundary.
 */
class ModeISurfingDirichletBC : public NodalBC
{
public:
  ModeISurfingDirichletBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;

  const VariableValue & _xc;
  const VariableValue & _yc;
  const unsigned int _component;
  const Real _Gc;
  const Real _E;
  const Real _nu;
};
