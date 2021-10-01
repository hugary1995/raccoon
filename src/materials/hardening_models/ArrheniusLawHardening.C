//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ArrheniusLawHardening.h"

registerMooseObject("raccoonApp", ArrheniusLawHardening);

InputParameters
ArrheniusLawHardening::validParams()
{
  InputParameters params = PlasticHardeningModel::validParams();
  params.addClassDescription("Plastic hardening following a temperature dependent Arrhenius law.");

  params.addRequiredParam<MaterialPropertyName>("reference_stress",
                                                "The reference stress $\\sigma_0$");
  params.addRequiredParam<MaterialPropertyName>("arrhenius_coefficient",
                                                "The arrhenius coefficient");
  params.addRequiredParam<Real>(
      "eps", "A small number to help this perfect plasticity model to converge.");

  params.addRequiredCoupledVar("phase_field", "Name of the phase-field (damage) variable");
  params.addParam<MaterialPropertyName>(
      "plastic_energy_density",
      "psip",
      "Name of the plastic energy density computed by this material model");
  params.addParam<MaterialPropertyName>("degradation_function", "gp", "The degradation function");
  params.addRangeCheckedParam<Real>(
      "taylor_quinney_factor",
      1,
      "taylor_quinney_factor<=1 & taylor_quinney_factor>=0",
      "The Taylor-Quinney factor. 1 (default) for purely dissipative, 0 for purely energetic.");

  params.addRequiredCoupledVar("temperature",
                               "Couples with temperature to compute heat generation/loss due to "
                               "hardening/softening.");

  return params;
}

ArrheniusLawHardening::ArrheniusLawHardening(const InputParameters & parameters)
  : PlasticHardeningModel(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _sigma_0(getADMaterialProperty<Real>(prependBaseName("reference_stress", true))),
    _arrhenius_coef_name(prependBaseName("arrhenius_coefficient", true)),
    _arrhenius_coef(getADMaterialProperty<Real>(_arrhenius_coef_name)),
    _darrhenius_coef_dT(getADMaterialProperty<Real>(
        derivativePropertyName(_arrhenius_coef_name, {getVar("temperature", 0)->name()}))),
    _eps(getParam<Real>("eps")),

    _d_name(getVar("phase_field", 0)->name()),

    // The plastic energy density and its derivatives
    _psip_name(prependBaseName("plastic_energy_density", true)),
    _psip(declareADProperty<Real>(_psip_name)),
    _psip_active(declareADProperty<Real>(_psip_name + "_active")),
    _dpsip_dd(declareADProperty<Real>(derivativePropertyName(_psip_name, {_d_name}))),

    // The degradation function and its derivatives
    _gp_name(prependBaseName("degradation_function", true)),
    _gp(getADMaterialProperty<Real>(_gp_name)),
    _dgp_dd(getADMaterialProperty<Real>(derivativePropertyName(_gp_name, {_d_name}))),

    // TQF
    _tqf(getParam<Real>("taylor_quinney_factor")),

    // Temperature
    _temp(adCoupledValue("temperature"))
{
}

ADReal
ArrheniusLawHardening::plasticEnergy(const ADReal & ep, const unsigned int derivative)
{
  if (derivative == 0)
  {
    _psip_active[_qp] =
        (1 - _tqf) * _sigma_0[_qp] / _arrhenius_coef[_qp] * ep + 0.5 * _eps * ep * ep;
    _psip[_qp] = _gp[_qp] * _psip_active[_qp];
    _dpsip_dd[_qp] = _dgp_dd[_qp] * _psip_active[_qp];
    return _psip[_qp];
  }

  if (derivative == 1)
    return (1 - _tqf) * _gp[_qp] * (_sigma_0[_qp] / _arrhenius_coef[_qp] + _eps * ep);

  if (derivative == 2)
    return (1 - _tqf) * _gp[_qp] * _eps;

  mooseError(name(), "internal error: unsupported derivative order.");
  return 0;
}

ADReal
ArrheniusLawHardening::plasticDissipation(const ADReal & delta_ep,
                                          const ADReal & ep,
                                          const unsigned int derivative)
{
  if (derivative == 0)
  {
    return _tqf * _gp[_qp] * (_sigma_0[_qp] / _arrhenius_coef[_qp] + _eps * ep) * delta_ep;
  }

  if (derivative == 1)
    return _tqf * _gp[_qp] * (_sigma_0[_qp] / _arrhenius_coef[_qp] + _eps * ep);

  if (derivative == 2)
    return _tqf * _gp[_qp] * _eps;

  mooseError(name(), "internal error: unsupported derivative order.");
  return 0;
}

ADReal
ArrheniusLawHardening::thermalConjugate(const ADReal & /*ep*/)
{
  return -_temp[_qp] * (1 - _tqf) * _gp[_qp] * _sigma_0[_qp] / _arrhenius_coef[_qp] /
         _arrhenius_coef[_qp] * _darrhenius_coef_dT[_qp];
}
