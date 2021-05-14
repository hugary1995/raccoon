//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PFFPowerLawHardening.h"

registerMooseObject("raccoonApp", PFFPowerLawHardening);

InputParameters
PFFPowerLawHardening::validParams()
{
  InputParameters params = PowerLawHardening::validParams();
  params.addRequiredCoupledVar("phase_field", "Name of the phase-field (damage) variable");
  params.addParam<MaterialPropertyName>(
      "plastic_energy_density",
      "wp",
      "Name of the plastic energy density computed by this material model");
  params.addParam<MaterialPropertyName>("degradation_function", "gp", "The degradation function");

  return params;
}

PFFPowerLawHardening::PFFPowerLawHardening(const InputParameters & parameters)
  : PowerLawHardening(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _d_name(getVar("phase_field", 0)->name()),

    // The strain energy density and its derivatives
    _wp_name(_base_name + getParam<MaterialPropertyName>("plastic_energy_density")),
    _wp(declareADProperty<Real>(_wp_name)),
    _wp_active(declareADProperty<Real>(_wp_name + "_active")),
    _dwp_dd(declareADProperty<Real>(derivativePropertyName(_wp_name, {_d_name}))),

    // The degradation function and its derivatives
    _gp_name(_base_name + getParam<MaterialPropertyName>("degradation_function")),
    _gp(getADMaterialProperty<Real>(_gp_name)),
    _dgp_dd(getADMaterialProperty<Real>(derivativePropertyName(_gp_name, {_d_name})))
{
}

ADReal
PFFPowerLawHardening::plasticEnergy(const ADReal & ep, const unsigned int derivative)
{
  if (derivative == 0)
  {
    _wp_active[_qp] = PowerLawHardening::plasticEnergy(ep);
    _wp[_qp] = _gp[_qp] * _wp_active[_qp];
    _dwp_dd[_qp] = _dgp_dd[_qp] * _wp_active[_qp];
    return _wp[_qp];
  }

  return _gp[_qp] * PowerLawHardening::plasticEnergy(ep, derivative);
}
