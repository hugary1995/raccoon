//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LinearDegradation.h"

registerADMooseObject("raccoonApp", LinearDegradation);

template <bool is_ad>
InputParameters
LinearDegradationTempl<is_ad>::validParams()
{
  InputParameters params = DegradationBaseTempl<is_ad>::validParams();
  params.addClassDescription("computes the degradation function of quadratic form, $(1 - d)$.");
  return params;
}

template <bool is_ad>
LinearDegradationTempl<is_ad>::LinearDegradationTempl(const InputParameters & parameters)
  : DegradationBaseTempl<is_ad>(parameters)
{
}

template <bool is_ad>
void
LinearDegradationTempl<is_ad>::computeDegradation()
{
  GenericReal<is_ad> d = _d[_qp];
  GenericReal<is_ad> d_old = _lag ? _d_old[_qp] : d;
  _g[_qp] = 1.0 - d_old;
  _dg_dd[_qp] = -1.0;
}

template class LinearDegradationTempl<true>;
