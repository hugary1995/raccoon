//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ExponentialHardening.h"

registerMooseObject("raccoonApp", ExponentialHardening);

InputParameters
ExponentialHardening::validParams()
{
  InputParameters params = PlasticHardeningModel::validParams();
  params.addClassDescription("Plastic hardening in exponential form.");

  params.addRequiredParam<MaterialPropertyName>("yield_stress", "The yield stress $\\sigma_y$");
  params.addRequiredParam<MaterialPropertyName>("ultimate_stress",
                                                "The ultimate stress $\\sigma_\\infty$");
  params.addRequiredParam<MaterialPropertyName>(
      "reference_plastic_strain", "The $\\epsilon_0$ parameter in the exponential hardening");
  params.addRequiredParam<MaterialPropertyName>("hardening_modulus", "The hardening modulus $H$");

  params.addRequiredCoupledVar("phase_field", "Name of the phase-field (damage) variable");
  params.addParam<MaterialPropertyName>(
      "plastic_energy_density",
      "psip",
      "Name of the plastic energy density computed by this material model");
  params.addParam<MaterialPropertyName>("degradation_function", "gp", "The degradation function");

  return params;
}

ExponentialHardening::ExponentialHardening(const InputParameters & parameters)
  : PlasticHardeningModel(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _sigma_y(getADMaterialProperty<Real>(prependBaseName("yield_stress", true))),
    _sigma_ult(getADMaterialProperty<Real>(prependBaseName("ultimate_stress", true))),
    _ep0(getADMaterialProperty<Real>(prependBaseName("reference_plastic_strain", true))),
    _H(getADMaterialProperty<Real>(prependBaseName("hardening_modulus", true))),

    _d_name(getVar("phase_field", 0)->name()),

    // The strain energy density and its derivatives
    _psip_name(prependBaseName("plastic_energy_density", true)),
    _psip(declareADProperty<Real>(_psip_name)),
    _psip_active(declareADProperty<Real>(_psip_name + "_active")),
    _dpsip_dd(declareADProperty<Real>(derivativePropertyName(_psip_name, {_d_name}))),

    // The degradation function and its derivatives
    _gp_name(prependBaseName("degradation_function", true)),
    _gp(getADMaterialProperty<Real>(_gp_name)),
    _dgp_dd(getADMaterialProperty<Real>(derivativePropertyName(_gp_name, {_d_name})))
{
}

ADReal
ExponentialHardening::plasticEnergy(const ADReal & ep, const unsigned int derivative)
{
  if (derivative == 0)
  {
    _psip_active[_qp] =
        _sigma_ult[_qp] * ep +
        _ep0[_qp] * (_sigma_ult[_qp] - _sigma_y[_qp]) * (std::exp(-ep / _ep0[_qp]) - 1) +
        0.5 * _H[_qp] * ep * ep;
    _psip[_qp] = _gp[_qp] * _psip_active[_qp];
    _dpsip_dd[_qp] = _dgp_dd[_qp] * _psip_active[_qp];
    return _psip[_qp];
  }

  if (derivative == 1)
    return _gp[_qp] *
           (_sigma_ult[_qp] - (_sigma_ult[_qp] - _sigma_y[_qp]) * std::exp(-ep / _ep0[_qp]) +
            _H[_qp] * ep);

  if (derivative == 2)
    return _gp[_qp] *
           ((_sigma_ult[_qp] - _sigma_y[_qp]) * std::exp(-ep / _ep0[_qp]) / _ep0[_qp] + _H[_qp]);

  mooseError(name(), "internal error: unsupported derivative order.");
  return 0;
}
