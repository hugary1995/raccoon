//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "NodalBC.h"

/**
 * Defines a boundary condition that forces the value to be a user specified
 * scalar at the boundary.
 */
class ModeISurfingDirichletBC : public NodalBC
{
public:
  static InputParameters validParams();

  ModeISurfingDirichletBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;

  const Point & _c;
  const RealVectorValue & _v;
  const unsigned int _component;
  const Real _Gc;
  const Real _K;
  const Real _G;
};
