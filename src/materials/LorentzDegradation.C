//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LorentzDegradation.h"

registerADMooseObject("raccoonApp", LorentzDegradation);

defineADValidParams(
    LorentzDegradation,
    DegradationBase,
    params.addParam<MaterialPropertyName>("mobility_name", "mobility", "name of the Mobility");
    params.addParam<MaterialPropertyName>("critical_fracture_energy_name",
                                          "critical_fracture_energy",
                                          "critical fracture energy");
    params.addParam<Real>("p",
                          1.0,
                          "shape parameter that controls the size of the fracture process zone"););

template <ComputeStage compute_stage>
LorentzDegradation<compute_stage>::LorentzDegradation(const InputParameters & parameters)
  : DegradationBase<compute_stage>(parameters),
    _M(getMaterialProperty<Real>("mobility_name")),
    _b(getMaterialProperty<Real>("critical_fracture_energy_name")),
    _p(getParam<Real>("p"))
{
}

template <ComputeStage compute_stage>
void
LorentzDegradation<compute_stage>::computeDegradation()
{
  ADReal M = _M[_qp];
  ADReal b = _b[_qp];

  // g
  ADReal d = _lag ? _d_old[_qp] : _d[_qp];
  ADReal num = (1.0 - d) * (1.0 - d);
  ADReal denom = num + M / b * d * (1.0 + _p * d);
  _g[_qp] = num / denom;

  // dg_dd
  d = _d[_qp];
  num = (1.0 - d) * (1.0 - d);
  denom = num + M / b * d * (1.0 + _p * d);
  ADReal dnum_dd = -2.0 * (1.0 - d);
  ADReal ddenom_dd = dnum_dd + M / b * (1.0 + 2.0 * _p * d);
  _dg_dd[_qp] = (dnum_dd * denom - num * ddenom_dd) / denom / denom;
}
