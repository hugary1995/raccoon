//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LinearLocalDissipation.h"

registerADMooseObject("raccoonApp", LinearLocalDissipation);

template <bool is_ad>
InputParameters
LinearLocalDissipationTempl<is_ad>::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("computes the local dissipation function of the linear form, $d$");
  params.addRequiredCoupledVar("d", "phase-field damage variable");
  params.addParam<MaterialPropertyName>(
      "local_dissipation_name", "w", "name of the material that holds the local dissipation");
  return params;
}

template <bool is_ad>
LinearLocalDissipationTempl<is_ad>::LinearLocalDissipationTempl(const InputParameters & parameters)
  : Material(parameters),
    _d(coupledGenericValue<is_ad>("d")),
    _w_name(getParam<MaterialPropertyName>("local_dissipation_name")),
    _w(declareGenericProperty<Real, is_ad>(_w_name)),
    _dw_dd(declareGenericProperty<Real, is_ad>(
        derivativePropertyNameFirst(_w_name, this->getVar("d", 0)->name())))
{
}

template <bool is_ad>
void
LinearLocalDissipationTempl<is_ad>::computeQpProperties()
{
  GenericReal<is_ad> d = _d[_qp];

  _w[_qp] = d;
  _dw_dd[_qp] = 1.0;
}

// template class LinearLocalDissipationTempl<false>;
template class LinearLocalDissipationTempl<true>;
