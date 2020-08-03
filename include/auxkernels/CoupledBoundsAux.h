//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "BoundsAuxBase.h"

/**
 * Provides a bound of a variable using a coupled variable.
 */
class CoupledBoundsAux : public BoundsAuxBase
{
public:
  static InputParameters validParams();

  CoupledBoundsAux(const InputParameters & parameters);

protected:
  virtual Real getBound() override;

  MooseVariable & _coupled_var;
};
