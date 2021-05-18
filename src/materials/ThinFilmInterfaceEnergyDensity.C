//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ThinFilmInterfaceEnergyDensity.h"

registerMooseObject("raccoonApp", ThinFilmInterfaceEnergyDensity);

InputParameters
ThinFilmInterfaceEnergyDensity::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription(
      "This class compute the interface energy density corresponding to a shear-lag model, "
      "i.e. $\\psi^i = 0.5 c \\bs{u} \\cdot \\bs{u}$.");

  params.addRequiredParam<MaterialPropertyName>(
      "shear_lag_coef",
      "The coefficient describing the mismatch between the film and the substrate");
  params.addRequiredCoupledVar(
      "displacements",
      "The displacements appropriate for the simulation geometry and coordinate system");

  params.addRequiredCoupledVar("phase_field", "Name of the phase-field (damage) variable");
  params.addParam<MaterialPropertyName>(
      "interface_energy_density", "psii", "Name of the interfacial energy density");
  params.addParam<MaterialPropertyName>("degradation_function", "g", "The degradation function");

  return params;
}

ThinFilmInterfaceEnergyDensity::ThinFilmInterfaceEnergyDensity(const InputParameters & parameters)
  : Material(parameters),
    DerivativeMaterialPropertyNameInterface(),
    BaseNameInterface(parameters),
    _coef(getADMaterialProperty<Real>(prependBaseName("shear_lag_coef", true))),
    _ndisp(coupledComponents("displacements")),
    _disp(3),

    _d_name(getVar("phase_field", 0)->name()),

    // The strain energy density and its derivatives
    _psii_name(prependBaseName("interface_energy_density", true)),
    _psii(declareADProperty<Real>(_psii_name)),
    _psii_active(declareADProperty<Real>(_psii_name + "_active")),
    _dpsii_dd(declareADProperty<Real>(derivativePropertyName(_psii_name, {_d_name}))),

    // The degradation function and its derivatives
    _g_name(prependBaseName("degradation_function", true)),
    _g(getADMaterialProperty<Real>(_g_name)),
    _dg_dd(getADMaterialProperty<Real>(derivativePropertyName(_g_name, {_d_name})))
{
  for (unsigned int i = 0; i < _ndisp; ++i)
    _disp[i] = &adCoupledValue("displacements", i);

  // set unused dimensions to zero
  for (unsigned i = _ndisp; i < 3; ++i)
    _disp[i] = &_ad_zero;
}

void
ThinFilmInterfaceEnergyDensity::computeQpProperties()
{
  ADRealVectorValue u((*_disp[0])[_qp], (*_disp[1])[_qp], (*_disp[2])[_qp]);
  _psii_active[_qp] = 0.5 * _coef[_qp] * u * u;
  _psii[_qp] = _g[_qp] * _psii_active[_qp];
  _dpsii_dd[_qp] = _dg_dd[_qp] * _psii_active[_qp];
}
