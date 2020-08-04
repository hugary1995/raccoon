//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "NoDegradation.h"

registerADMooseObject("raccoonApp", NoDegradation);

template <bool is_ad>
InputParameters
NoDegradationTempl<is_ad>::validParams()
{
  InputParameters params = DegradationBaseTempl<is_ad>::validParams();
  params.addClassDescription("defines a dummy degradation function $g(d) = 1$");
  return params;
}

template <bool is_ad>
NoDegradationTempl<is_ad>::NoDegradationTempl(const InputParameters & parameters)
  : DegradationBaseTempl<is_ad>(parameters)
{
}

template <bool is_ad>
void
NoDegradationTempl<is_ad>::computeDegradation()
{
  _g[_qp] = 1;
  _dg_dd[_qp] = 0;
}

template class NoDegradationTempl<true>;
