//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LinearDegradation.h"

registerADMooseObject("raccoonApp", LinearDegradation);

InputParameters
LinearDegradation::validParams()
{
  InputParameters params = DegradationBase::validParams();
  params.addClassDescription("computes the degradation function of quadratic form, $(1 - d)$.");
  return params;
}

LinearDegradation::LinearDegradation(const InputParameters & parameters)
  : DegradationBase(parameters)
{
}

void
LinearDegradation::computeDegradation()
{
  ADReal d = _d[_qp];
  ADReal d_old = _lag ? _d_old[_qp] : d;
  _g[_qp] = 1.0 - d_old;
  _dg_dd[_qp] = -1.0;
}
