//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "DamageExtrapolation.h"
#include "NonlinearSystem.h"

registerMooseObject("raccoonApp", DamageExtrapolation);

template <>
InputParameters
validParams<DamageExtrapolation>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("d",
                               "damage variable to be extrapolated between nonlinear iterations");
  return params;
}

DamageExtrapolation::DamageExtrapolation(const InputParameters & parameters)
  : AuxKernel(parameters),
    _d(coupledValue("d")),
    _d_old(coupledValueOld("d")),
    _d_older(coupledValueOlder("d"))
{
}

Real
DamageExtrapolation::computeValue()
{
  Real d_predicted = 3.0 * (_d[_qp] - _d_old[_qp]) + _d_older[_qp];

  d_predicted = std::min(d_predicted, 1.0);
  d_predicted = std::max(d_predicted, 0.0);

  return d_predicted;
}
