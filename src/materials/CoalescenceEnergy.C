//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CoalescenceEnergy.h"

registerMooseObject("raccoonApp", CoalescenceEnergy);

InputParameters
CoalescenceEnergy::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params.addClassDescription("computes coalescence mobility of the form $g^p(d) c M e^p w(d)$");
  params.addParam<MaterialPropertyName>(
      "degradation_name", "g", "name of the degradation material");
  params.addParam<MaterialPropertyName>(
      "local_dissipation_name", "w", "name of the local dissipation function");
  params.addParam<MaterialPropertyName>(
      "mobility_name", "coalescence_mobility", "name of the material that holds the mobility");
  params.addParam<MaterialPropertyName>(
      "base_mobility_name", "mobility", "name of the material that holds the base mobility");
  params.addParam<MaterialPropertyName>(
      "coalescence_energy_name",
      "E_coalesce",
      "name of the material that holds the undegraded coalescence energy");
  params.addRequiredParam<Real>("coalescence_coefficient", "coalescence coefficient");
  params.addParam<std::string>("base_name", "base name of the effective plastic strain");

  return params;
}

CoalescenceEnergy::CoalescenceEnergy(const InputParameters & parameters)
  : ADMaterial(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _c(getParam<Real>("coalescence_coefficient")),
    _M(declareADProperty<Real>(getParam<MaterialPropertyName>("mobility_name"))),
    _M0(getADMaterialProperty<Real>("base_mobility_name")),
    _E_coalesce(declareADProperty<Real>(getParam<MaterialPropertyName>("coalescence_energy_name"))),
    _g(getADMaterialProperty<Real>("degradation_name")),
    _w(getADMaterialProperty<Real>("local_dissipation_name")),
    _ep(getADMaterialProperty<Real>(_base_name + "effective_plastic_strain"))
{
}

void
CoalescenceEnergy::initQpStatefulProperties()
{
  computeQpProperties();
}

void
CoalescenceEnergy::computeQpProperties()
{
  _M[_qp] = _g[_qp] * _c * _M0[_qp] * _ep[_qp];
  _E_coalesce[_qp] = _c * _M0[_qp] * _ep[_qp] * _w[_qp];
}
