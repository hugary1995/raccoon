//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "QuasiLinearDegradation.h"

registerADMooseObject("raccoonApp", QuasiLinearDegradation);

defineADLegacyParams(QuasiLinearDegradation);

template <ComputeStage compute_stage>
InputParameters
QuasiLinearDegradation<compute_stage>::validParams()
{
  InputParameters params = DegradationBase<compute_stage>::validParams();
  params.addClassDescription("computes the quasi-linear Lorentz-type degradation: "
                             "$\\frac{(1-d)}{(1-d)+\\frac{M}{\\psi_\\critical}d}$");
  params.addParam<MaterialPropertyName>("mobility_name", "mobility", "name of the Mobility");
  params.addParam<MaterialPropertyName>(
      "critical_fracture_energy_name", "critical_fracture_energy", "critical fracture energy");
  return params;
}

template <ComputeStage compute_stage>
QuasiLinearDegradation<compute_stage>::QuasiLinearDegradation(const InputParameters & parameters)
  : DegradationBase<compute_stage>(parameters),
    _M(getMaterialProperty<Real>("mobility_name")),
    _b(getMaterialProperty<Real>("critical_fracture_energy_name"))
{
}

template <ComputeStage compute_stage>
void
QuasiLinearDegradation<compute_stage>::computeDegradation()
{
  ADReal M = _M[_qp];
  ADReal b = _b[_qp];

  // g
  ADReal d = _lag ? _d_old[_qp] : _d[_qp];
  ADReal num = 1.0 - d;
  ADReal denom = num + M / b * d;
  _g[_qp] = num / denom;

  // dg_dd
  d = _d[_qp];
  num = 1.0 - d;
  denom = num + M / b * d;
  ADReal dnum_dd = -d;
  ADReal ddenom_dd = dnum_dd + M / b;
  _dg_dd[_qp] = (dnum_dd * denom - num * ddenom_dd) / denom / denom;
}
