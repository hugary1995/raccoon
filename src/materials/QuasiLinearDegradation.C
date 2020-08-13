//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "QuasiLinearDegradation.h"

registerADMooseObject("raccoonApp", QuasiLinearDegradation);

template <bool is_ad>
InputParameters
QuasiLinearDegradationTempl<is_ad>::validParams()
{
  InputParameters params = DegradationBaseTempl<is_ad>::validParams();
  params.addClassDescription("computes the quasi-linear Lorentz-type degradation: "
                             "$\\frac{(1-d)}{(1-d)+\\frac{M}{\\psi_\\critical}d}$");
  params.addParam<MaterialPropertyName>("mobility_name", "mobility", "name of the Mobility");
  params.addParam<MaterialPropertyName>(
      "critical_fracture_energy_name", "critical_fracture_energy", "critical fracture energy");
  params.addRangeCheckedParam<Real>(
      "xi", 1.0, "xi > 0 & xi <= 2", "derivative of te local dissipation function at d = 0");
  return params;
}

template <bool is_ad>
QuasiLinearDegradationTempl<is_ad>::QuasiLinearDegradationTempl(const InputParameters & parameters)
  : DegradationBaseTempl<is_ad>(parameters),
    _M(getGenericMaterialProperty<Real, is_ad>(getParam<MaterialPropertyName>("mobility_name"))),
    _b(getGenericMaterialProperty<Real, is_ad>(
        getParam<MaterialPropertyName>("critical_fracture_energy_name"))),
    _xi(getParam<Real>("xi"))
{
}

template <bool is_ad>
void
QuasiLinearDegradationTempl<is_ad>::computeDegradation()
{
  GenericReal<is_ad> M = _M[_qp];
  GenericReal<is_ad> b = _b[_qp];

  // g
  GenericReal<is_ad> d = _lag ? _d_old[_qp] : _d[_qp];
  GenericReal<is_ad> num = 1.0 - d;
  GenericReal<is_ad> denom = num + M * _xi / b * d;
  _g[_qp] = num / denom;

  // dg_dd
  d = _d[_qp];
  num = 1.0 - d;
  denom = num + M * _xi / b * d;
  GenericReal<is_ad> dnum_dd = -d;
  GenericReal<is_ad> ddenom_dd = dnum_dd + M * _xi / b;
  _dg_dd[_qp] = (dnum_dd * denom - num * ddenom_dd) / denom / denom;
}

template class QuasiLinearDegradationTempl<true>;
