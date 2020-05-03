//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "QuadraticLocalDissipation.h"

registerADMooseObject("raccoonApp", QuadraticLocalDissipation);

InputParameters
QuadraticLocalDissipation::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params.addRequiredCoupledVar("d", "phase-field damage variable");
  params.addParam<MaterialPropertyName>(
      "local_dissipation_name", "w", "name of the material that holds the local dissipation");
  params.addRangeCheckedParam<Real>(
      "xi", 0.0, "xi >= 0 & xi <= 2", "derivative of te local dissipation function at d = 0");
  return params;
}

QuadraticLocalDissipation::QuadraticLocalDissipation(const InputParameters & parameters)
  : ADMaterial(parameters),
    _d(adCoupledValue("d")),
    _w_name(getParam<MaterialPropertyName>("local_dissipation_name")),
    _w(declareADProperty<Real>(_w_name)),
    _dw_dd(declareADProperty<Real>(
        derivativePropertyNameFirst(_w_name, this->getVar("d", 0)->name()))),
    _xi(getParam<Real>("xi"))
{
}

void
QuadraticLocalDissipation::computeQpProperties()
{
  ADReal d = _d[_qp];

  _w[_qp] = (1.0 - _xi) * d * d + _xi * d;
  _dw_dd[_qp] = 2.0 * (1.0 - _xi) * d + _xi;
}
