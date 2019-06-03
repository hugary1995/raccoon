//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "DegradationBase.h"

defineADValidParams(
    DegradationBase, ADMaterial, params.addRequiredCoupledVar("d", "phase-field damage variable");
    params.addParam<MaterialPropertyName>(
        "degradation_name", "g", "name of the material that holds the degradation function value");
    params.addParam<Real>("residual_degradation", 1e-6, "residual degradation");
    params.addParam<bool>("lag_degradation",
                          false,
                          "whether to use the last converged damage variable to compute "
                          "degradation (the derivative of degradation is always NOT lagged)"));

template <ComputeStage compute_stage>
DegradationBase<compute_stage>::DegradationBase(const InputParameters & parameters)
  : ADMaterial<compute_stage>(parameters),
    _d(adCoupledValue("d")),
    _d_old(coupledValueOld("d")),
    _g_name(adGetParam<MaterialPropertyName>("degradation_name")),
    _g(adDeclareADProperty<Real>(_g_name)),
    _dg_dd(adDeclareADProperty<Real>(
        derivativePropertyNameFirst(_g_name, this->getVar("d", 0)->name()))),
    _eta(adGetParam<Real>("residual_degradation")),
    _lag(adGetParam<bool>("lag_degradation"))
{
}

template <ComputeStage compute_stage>
void
DegradationBase<compute_stage>::computeQpProperties()
{
  computeDegradation();
  postComputeDegradation();
}

template <ComputeStage compute_stage>
void
DegradationBase<compute_stage>::postComputeDegradation()
{
  _g[_qp] = _g[_qp] * (1.0 - _eta) + _eta;
  _dg_dd[_qp] = _dg_dd[_qp] * (1.0 - _eta);
}

adBaseClass(DegradationBase);
