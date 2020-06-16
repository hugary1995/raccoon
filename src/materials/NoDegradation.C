//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "NoDegradation.h"

registerADMooseObject("raccoonApp", NoDegradation);

InputParameters
NoDegradation::validParams()
{
  InputParameters params = DegradationBase::validParams();
  params.addClassDescription("defines a dummy degradation function $g(d) = 1$");
  return params;
}

NoDegradation::NoDegradation(const InputParameters & parameters) : DegradationBase(parameters) {}

void
NoDegradation::computeDegradation()
{
  _g[_qp] = 1;
  _dg_dd[_qp] = 0;
}
