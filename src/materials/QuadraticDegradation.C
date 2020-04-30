//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "QuadraticDegradation.h"

registerADMooseObject("raccoonApp", QuadraticDegradation);

InputParameters
QuadraticDegradation::validParams()
{
  InputParameters params = DegradationBase::validParams();
  params.addClassDescription("computes the degradation function of quadratic form, $(1 - d) ^ 2$.");
  return params;
}

QuadraticDegradation::QuadraticDegradation(const InputParameters & parameters)
  : DegradationBase(parameters)
{
}

void
QuadraticDegradation::computeDegradation()
{
  ADReal d = _d[_qp];
  ADReal d_old = _lag ? _d_old[_qp] : d;
  _g[_qp] = (1.0 - d_old) * (1.0 - d_old);
  _dg_dd[_qp] = -2.0 * (1.0 - d);
}
