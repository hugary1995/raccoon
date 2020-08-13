//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "QuadraticLocalDissipation.h"

registerADMooseObject("raccoonApp", QuadraticLocalDissipation);

template <bool is_ad>
InputParameters
QuadraticLocalDissipationTempl<is_ad>::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription(
      "computes the local dissipation potential of the form $\\xi d + (1 - \\xi) d$.");
  params.addRequiredCoupledVar("d", "phase-field damage variable");
  params.addParam<MaterialPropertyName>(
      "local_dissipation_name", "w", "name of the material that holds the local dissipation");
  params.addRangeCheckedParam<Real>(
      "xi", 0.0, "xi >= 0 & xi <= 2", "derivative of te local dissipation function at d = 0");
  return params;
}

template <bool is_ad>
QuadraticLocalDissipationTempl<is_ad>::QuadraticLocalDissipationTempl(
    const InputParameters & parameters)
  : Material(parameters),
    _d(coupledGenericValue<is_ad>("d")),
    _w_name(getParam<MaterialPropertyName>("local_dissipation_name")),
    _w(declareGenericProperty<Real, is_ad>(_w_name)),
    _dw_dd(declareGenericProperty<Real, is_ad>(
        derivativePropertyNameFirst(_w_name, this->getVar("d", 0)->name()))),
    _xi(getParam<Real>("xi"))
{
}

template <bool is_ad>
void
QuadraticLocalDissipationTempl<is_ad>::computeQpProperties()
{
  GenericReal<is_ad> d = _d[_qp];

  _w[_qp] = (1.0 - _xi) * d * d + _xi * d;
  _dw_dd[_qp] = 2.0 * (1.0 - _xi) * d + _xi;
}

// template class QuadraticLocalDissipationTempl<false>;
template class QuadraticLocalDissipationTempl<true>;
