//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "QuadraticDegradation.h"

registerADMooseObject("raccoonApp", QuadraticDegradation);

defineADValidParams(QuadraticDegradation, DegradationBase, );

template <ComputeStage compute_stage>
QuadraticDegradation<compute_stage>::QuadraticDegradation(const InputParameters & parameters)
  : DegradationBase<compute_stage>(parameters)
{
}

template <ComputeStage compute_stage>
void
QuadraticDegradation<compute_stage>::computeDegradation(ADReal & d)
{
  _g[_qp] = (1.0 - d) * (1.0 - d) * (1.0 - _eta) + _eta;
  _dg_dd[_qp] = -2.0 * (1.0 - d) * (1.0 - _eta);
}
