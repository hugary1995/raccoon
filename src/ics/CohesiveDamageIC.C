//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CohesiveDamageIC.h"

registerMooseObject("raccoonApp", CohesiveDamageIC);

template <>
InputParameters
validParams<CohesiveDamageIC>()
{
  InputParameters params = validParams<DamageICBase>();
  return params;
}

CohesiveDamageIC::CohesiveDamageIC(const InputParameters & parameters) : DamageICBase(parameters) {}

Real
CohesiveDamageIC::value(const Point & p)
{
  Real damage = 0.0;

  Real s = dist(p);
  if (s <= _l * 2.0)
    damage = _d0 * (1.0 - s / _l / 2.0) * (1.0 - s / _l / 2.0);

  damage = std::min(damage, 1.0);

  return damage;
}
