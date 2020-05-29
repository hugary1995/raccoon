//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "WuDegradation.h"

registerADMooseObject("raccoonApp", WuDegradation);

InputParameters
WuDegradation::validParams()
{
  InputParameters params = DegradationBase::validParams();
  params.addClassDescription("computes the Wu-type degradation: "
                             "$\\frac{(1-d)^2}{(1-d)^2+a_1d(1+a_2d(1+a_3d))}$");
  params.addParam<MaterialPropertyName>("mobility_name", "mobility", "name of the Mobility");
  params.addParam<MaterialPropertyName>(
      "critical_fracture_energy_name", "critical_fracture_energy", "critical fracture energy");
  params.addRequiredParam<Real>("a2", "parameter a_2");
  params.addRequiredParam<Real>("a3", "parameter a_3");
  params.addRangeCheckedParam<Real>(
      "xi", 1.0, "xi > 0 & xi <= 2", "derivative of te local dissipation function at d = 0");

  return params;
}

WuDegradation::WuDegradation(const InputParameters & parameters)
  : DegradationBase(parameters),
    _M(getADMaterialProperty<Real>("mobility_name")),
    _b(getMaterialProperty<Real>("critical_fracture_energy_name")),
    _a2(getParam<Real>("a2")),
    _a3(getParam<Real>("a3")),
    _xi(getParam<Real>("xi"))
{
}

void
WuDegradation::computeDegradation()
{
  ADReal M = _M[_qp];
  ADReal b = _b[_qp];

  // g
  ADReal d = _lag ? _d_old[_qp] : _d[_qp];
  ADReal num = (1.0 - d) * (1.0 - d);
  ADReal denom = num + _xi * M / b * d * (1.0 + _a2 * d * (1 + _a3 * d));
  _g[_qp] = num / denom;

  // dg_dd
  d = _d[_qp];
  num = (1.0 - d) * (1.0 - d);
  denom = num + _xi * M / b * d * (1.0 + _a2 * d * (1 + _a3 * d));
  ADReal dnum_dd = -2.0 * (1.0 - d);
  ADReal ddenom_dd = dnum_dd + _xi * M / b * (_a2 * d * (3 * _a3 * d + 2) + 1);
  _dg_dd[_qp] = (dnum_dd * denom - num * ddenom_dd) / denom / denom;
}
