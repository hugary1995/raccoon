//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CohesiveDamageIC.h"

registerMooseObject("raccoonApp", CohesiveDamageIC);

InputParameters
CohesiveDamageIC::validParams()
{
  InputParameters params = DamageICBase::validParams();
  params.addClassDescription(
      "initializes the phase-field with the closed-form solution associated with a linear local "
      "dissipation potential, i.e. $d(\\tau) = (1 - \\frac{\\tau}{2l})^2$.");
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
