//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "DirichletBCBase.h"

/**
 * Defines a boundary condition that forces the value to be a user specified
 * function at the boundary.
 */
class LoadingUnloadingDirichletBC : public DirichletBCBase
{
public:
  static InputParameters validParams();

  LoadingUnloadingDirichletBC(const InputParameters & parameters);

protected:
  virtual void timestepSetup() override;
  virtual Real computeQpValue() override;

  Real _load;
  Real _load_cap;
  const Real _load_cap_inc;
  const Real _load_step;
  const Real _ultimate_load;
  const PostprocessorValue & _unloaded_indicator;
  bool _loading;
};
