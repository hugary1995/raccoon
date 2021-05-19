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

  return params;
}

ArrheniusLawHardening::ArrheniusLawHardening(const InputParameters & parameters)
  : PlasticHardeningModel(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _sigma_0(getADMaterialProperty<Real>(prependBaseName("reference_stress", true))),
    _arrhenius_coef(getADMaterialProperty<Real>(prependBaseName("arrhenius_coefficient", true))),
    _eps(getParam<Real>("eps")),

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
ArrheniusLawHardening::plasticEnergy(const ADReal & ep, const unsigned int derivative)
{
  if (derivative == 0)
  {
    _psip_active[_qp] = _sigma_0[_qp] / _arrhenius_coef[_qp] * ep + 0.5 * _eps * ep * ep;
    _psip[_qp] = _gp[_qp] * _psip_active[_qp];
    _dpsip_dd[_qp] = _dgp_dd[_qp] * _psip_active[_qp];
    return _psip[_qp];
  }

  if (derivative == 1)
    return _gp[_qp] * (_sigma_0[_qp] / _arrhenius_coef[_qp] + _eps * ep);

  if (derivative == 2)
    return _gp[_qp] * _eps;

  mooseError(name(), "internal error: unsupported derivative order.");
  return 0;
}
