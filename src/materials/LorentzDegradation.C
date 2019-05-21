#include "LorentzDegradation.h"

registerADMooseObject("raccoonApp", LorentzDegradation);

defineADValidParams(
    LorentzDegradation,
    DegradationBase,
    params.addParam<MaterialPropertyName>("mobility_name", "M", "name of the Mobility");
    params.addParam<MaterialPropertyName>("fracture_energy_barrier_name",
                                          "b",
                                          "critical fracture energy");
    params.addParam<Real>("p",
                          1.0,
                          "shape parameter that controls the size of the fracture process zone"););

template <ComputeStage compute_stage>
LorentzDegradation<compute_stage>::LorentzDegradation(const InputParameters & parameters)
  : DegradationBase<compute_stage>(parameters),
    _M(adGetMaterialProperty<Real>("mobility_name")),
    _b(adGetMaterialProperty<Real>("fracture_energy_barrier_name")),
    _p(adGetParam<Real>("p"))
{
}

template <ComputeStage compute_stage>
void
LorentzDegradation<compute_stage>::computeDegradation(ADReal & d)
{
  ADReal M = _M[_qp];
  ADReal b = _b[_qp];

  // g
  ADReal num = (1.0 - d) * (1.0 - d);
  ADReal denom = num + M / b * d * (1.0 + _p * d);
  _g[_qp] = num / denom;

  // dg_dd
  ADReal dnum_dd = -2.0 * (1.0 - d);
  ADReal ddenom_dd = dnum_dd + M / b * (1.0 + 2.0 * _p * d);
  _dg_dd[_qp] = (dnum_dd * denom - num * ddenom_dd) / denom / denom;
}
