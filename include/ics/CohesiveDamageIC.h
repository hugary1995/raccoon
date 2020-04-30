//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "DamageICBase.h"

class CohesiveDamageIC : public DamageICBase
{
public:
  static InputParameters validParams();

  CohesiveDamageIC(const InputParameters & parameters);

protected:
  virtual Real value(const Point & p);
};
