//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LorentzDegradation.h"

registerADMooseObject("raccoonApp", LorentzDegradation);

InputParameters
LorentzDegradation::validParams()
{
  InputParameters params = DegradationBase::validParams();
  params.addClassDescription("computes the Lorentz-type degradation: "
                             "$\\frac{(1-d)^2}{(1-d)^2+\\frac{M}{\\psi_\\critical}d(1+pd)}$");
  params.addParam<MaterialPropertyName>("mobility_name", "mobility", "name of the Mobility");
  params.addParam<MaterialPropertyName>(
      "critical_fracture_energy_name", "critical_fracture_energy", "critical fracture energy");
  params.addRangeCheckedParam<Real>(
      "p", 1.0, "p >= 1", "shape parameter that controls the size of the fracture process zone");
  params.addRangeCheckedParam<Real>(
      "xi", 1.0, "xi > 0 & xi <= 2", "derivative of te local dissipation function at d = 0");
  return params;
}

LorentzDegradation::LorentzDegradation(const InputParameters & parameters)
  : DegradationBase(parameters),
    _M(getADMaterialProperty<Real>("mobility_name")),
    _b(getMaterialProperty<Real>("critical_fracture_energy_name")),
    _p(getParam<Real>("p")),
    _xi(getParam<Real>("xi"))
{
}

void
LorentzDegradation::computeDegradation()
{
  ADReal M = _M[_qp];
  ADReal b = _b[_qp];

  // g
  ADReal d = _lag ? _d_old[_qp] : _d[_qp];
  ADReal num = (1.0 - d) * (1.0 - d);
  ADReal denom = num + M * _xi / b * d * (1.0 + _p * d);
  _g[_qp] = num / denom;

  // dg_dd
  d = _d[_qp];
  num = (1.0 - d) * (1.0 - d);
  denom = num + M * _xi / b * d * (1.0 + _p * d);
  ADReal dnum_dd = -2.0 * (1.0 - d);
  ADReal ddenom_dd = dnum_dd + M * _xi / b * (1.0 + 2.0 * _p * d);
  _dg_dd[_qp] = (dnum_dd * denom - num * ddenom_dd) / denom / denom;
}
