//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "QuadraticDegradation.h"

registerADMooseObject("raccoonApp", QuadraticDegradation);

template <bool is_ad>
InputParameters
QuadraticDegradationTempl<is_ad>::validParams()
{
  InputParameters params = DegradationBaseTempl<is_ad>::validParams();
  params.addClassDescription("computes the degradation function of quadratic form, $(1 - d) ^ 2$.");
  return params;
}

template <bool is_ad>
QuadraticDegradationTempl<is_ad>::QuadraticDegradationTempl(const InputParameters & parameters)
  : DegradationBaseTempl<is_ad>(parameters)
{
}

template <bool is_ad>
void
QuadraticDegradationTempl<is_ad>::computeDegradation()
{
  GenericReal<is_ad> d = _d[_qp];
  GenericReal<is_ad> d_old = _lag ? _d_old[_qp] : d;
  _g[_qp] = (1.0 - d_old) * (1.0 - d_old);
  _dg_dd[_qp] = -2.0 * (1.0 - d);
}

template class QuadraticDegradationTempl<true>;
