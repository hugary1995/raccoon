//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PowerLawHardening.h"

registerMooseObject("raccoonApp", PowerLawHardening);

InputParameters
PowerLawHardening::validParams()
{
  InputParameters params = PlasticHardeningModel::validParams();
  params.addRequiredParam<MaterialPropertyName>("yield_stress", "The yield stress $\\sigma_y$");
  params.addRequiredParam<MaterialPropertyName>("exponent",
                                                "The exponent n in the power law hardening $n$");
  params.addRequiredParam<MaterialPropertyName>(
      "reference_plastic_strain", "The $\\epsilon_0$ parameter in the power law hardening");

  params.addRequiredCoupledVar("phase_field", "Name of the phase-field (damage) variable");
  params.addParam<MaterialPropertyName>(
      "plastic_energy_density",
      "psip",
      "Name of the plastic energy density computed by this material model");
  params.addParam<MaterialPropertyName>("degradation_function", "gp", "The degradation function");

  return params;
}

PowerLawHardening::PowerLawHardening(const InputParameters & parameters)
  : PlasticHardeningModel(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _sigma_y(getADMaterialProperty<Real>("yield_stress")),
    _n(getADMaterialProperty<Real>("exponent")),
    _ep0(getADMaterialProperty<Real>("reference_plastic_strain")),

    _d_name(getVar("phase_field", 0)->name()),

    // The strain energy density and its derivatives
    _psip_name(_base_name + getParam<MaterialPropertyName>("plastic_energy_density")),
    _psip(declareADProperty<Real>(_psip_name)),
    _psip_active(declareADProperty<Real>(_psip_name + "_active")),
    _dpsip_dd(declareADProperty<Real>(derivativePropertyName(_psip_name, {_d_name}))),

    // The degradation function and its derivatives
    _gp_name(_base_name + getParam<MaterialPropertyName>("degradation_function")),
    _gp(getADMaterialProperty<Real>(_gp_name)),
    _dgp_dd(getADMaterialProperty<Real>(derivativePropertyName(_gp_name, {_d_name})))
{
}

ADReal
PowerLawHardening::plasticEnergy(const ADReal & ep, const unsigned int derivative)
{
  if (derivative == 0)
  {
    _psip_active[_qp] = _n[_qp] * _sigma_y[_qp] * _ep0[_qp] / (_n[_qp] + 1) *
                        (std::pow(1 + ep / _ep0[_qp], 1 / _n[_qp] + 1) - 1);
    _psip[_qp] = _gp[_qp] * _psip_active[_qp];
    _dpsip_dd[_qp] = _dgp_dd[_qp] * _psip_active[_qp];
    return _psip[_qp];
  }

  if (derivative == 1)
    return _gp[_qp] * _sigma_y[_qp] * std::pow(1 + ep / _ep0[_qp], 1 / _n[_qp]);

  if (derivative == 2)
    return _gp[_qp] * _sigma_y[_qp] * std::pow(1 + ep / _ep0[_qp], 1 / _n[_qp] - 1) / _n[_qp] /
           _ep0[_qp];

  mooseError(name(), "internal error: unsupported derivative order.");
  return 0;
}
