//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "DamageICBase.h"

class BrittleDamageIC : public DamageICBase
{
public:
  static InputParameters validParams();

  BrittleDamageIC(const InputParameters & parameters);

protected:
  virtual Real value(const Point & p);

  const Real _m;
};
