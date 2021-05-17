//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ThinFilmInterfaceEnergy.h"

registerMooseObject("raccoonApp", ThinFilmInterfaceEnergy);

InputParameters
ThinFilmInterfaceEnergy::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("Compute the interface energy corresponding to a shear-lag model, "
                             "i.e. $\\psi = 0.5 c \\bs{u} \\cdot \\bs{u}$.");
  params.addParam<std::string>("base_name",
                               "Optional parameter that allows the user to define "
                               "multiple mechanics material systems on the same "
                               "block, i.e. for multiple phases");
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

ThinFilmInterfaceEnergy::ThinFilmInterfaceEnergy(const InputParameters & parameters)
  : Material(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _coef(
        getADMaterialProperty<Real>(_base_name + getParam<MaterialPropertyName>("shear_lag_coef"))),
    _ndisp(coupledComponents("displacements")),
    _disp(3),

    _d_name(getVar("phase_field", 0)->name()),

    // The strain energy density and its derivatives
    _psii_name(_base_name + getParam<MaterialPropertyName>("interface_energy_density")),
    _psii(declareADProperty<Real>(_psii_name)),
    _psii_active(declareADProperty<Real>(_psii_name + "_active")),
    _dpsii_dd(declareADProperty<Real>(derivativePropertyName(_psii_name, {_d_name}))),

    // The degradation function and its derivatives
    _g_name(_base_name + getParam<MaterialPropertyName>("degradation_function")),
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
ThinFilmInterfaceEnergy::computeQpProperties()
{
  ADRealVectorValue u((*_disp[0])[_qp], (*_disp[1])[_qp], (*_disp[2])[_qp]);
  _psii_active[_qp] = 0.5 * _coef[_qp] * u * u;
  _psii[_qp] = _g[_qp] * _psii_active[_qp];
  _dpsii_dd[_qp] = _dg_dd[_qp] * _psii_active[_qp];
}
