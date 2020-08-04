//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "DegradationBase.h"
// #include "metaphysicl/raw_type.h"

template <bool is_ad>
InputParameters
DegradationBaseTempl<is_ad>::validParams()
{
  InputParameters params = Material::validParams();
  params.addCoupledVar("d", "phase-field damage variable");
  params.addCoupledVar("d_relaxed", "relaxed phase-field damage variable");
  params.addParam<MaterialPropertyName>(
      "degradation_name", "g", "name of the material that holds the degradation function value");
  params.addParam<Real>("residual_degradation", 1e-6, "residual degradation");
  params.addParam<bool>("lag_degradation",
                        false,
                        "whether to use the last converged damage variable to compute "
                        "degradation (the derivative of degradation is always NOT lagged)");
  return params;
}

template <bool is_ad>
DegradationBaseTempl<is_ad>::DegradationBaseTempl(const InputParameters & parameters)
  : Material(parameters),
    _d(coupledGenericValue<is_ad>("d")),
    _d_old(isParamValid("d_relaxed") ? coupledValue("d_relaxed") : coupledValueOld("d")),
    _g_name(getParam<MaterialPropertyName>("degradation_name")),
    _g(declareGenericProperty<Real, is_ad>(_g_name)),
    _dg_dd(declareGenericProperty<Real, is_ad>(
        derivativePropertyNameFirst(_g_name, this->getVar("d", 0)->name()))),
    _eta(getParam<Real>("residual_degradation")),
    _lag(getParam<bool>("lag_degradation"))
{
}

template <bool is_ad>
void
DegradationBaseTempl<is_ad>::computeQpProperties()
{
  computeDegradation();
  postComputeDegradation();
}

template <bool is_ad>
void
DegradationBaseTempl<is_ad>::postComputeDegradation()
{
  _g[_qp] = _g[_qp] * (1.0 - _eta) + _eta;
  _dg_dd[_qp] = _dg_dd[_qp] * (1.0 - _eta);
}

template class DegradationBaseTempl<false>;
template class DegradationBaseTempl<true>;
