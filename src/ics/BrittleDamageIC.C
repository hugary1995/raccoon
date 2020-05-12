//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "BrittleDamageIC.h"

registerMooseObject("raccoonApp", BrittleDamageIC);

InputParameters
BrittleDamageIC::validParams()
{
  InputParameters params = DamageICBase::validParams();
  params.addClassDescription("initializes the phase-field with the closed-form solution associated "
                             "with $w(d) = d^2$, i.e. $d(\\tau) = \\exp(-\\tau / l)$.");
  return params;
}

BrittleDamageIC::BrittleDamageIC(const InputParameters & parameters) : DamageICBase(parameters) {}

Real
BrittleDamageIC::value(const Point & p)
{
  Real damage = 0.0;

  Real s = dist(p);
  if (s <= _l)
    damage = _d0 * std::exp(-s / _l);

  damage = std::min(damage, 1.0);

  return damage;
}
