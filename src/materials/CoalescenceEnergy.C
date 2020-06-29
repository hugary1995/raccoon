//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CoalescenceEnergy.h"

registerMooseObject("raccoonApp", CoalescenceEnergy);

InputParameters
CoalescenceEnergy::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params.addClassDescription(
      "computes coalescence mobility of the form $\\beta M (1 - \\exp(-e_p/e_0)) \\w(d)$");
  params.addParam<MaterialPropertyName>(
      "local_dissipation_name", "w", "name of the local dissipation function");
  params.addParam<MaterialPropertyName>(
      "mobility_name", "coalescence_mobility", "name of the material that holds the mobility");
  params.addParam<MaterialPropertyName>(
      "base_mobility_name", "mobility", "name of the material that holds the base mobility");
  params.addRequiredParam<Real>("beta", "coalescence coefficient");
  params.addRequiredParam<Real>("e0", "characteristic plastic strain");
  params.addParam<std::string>("base_name", "base name of the effective plastic strain");

  return params;
}

CoalescenceEnergy::CoalescenceEnergy(const InputParameters & parameters)
  : ADMaterial(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _beta(getParam<Real>("beta")),
    _e0(getParam<Real>("e0")),
    _M(declareADProperty<Real>(getParam<MaterialPropertyName>("mobility_name"))),
    _M0(getADMaterialProperty<Real>("base_mobility_name")),
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
  _M[_qp] = _M0[_qp] * (1 + (1 - _beta) * (std::exp(-_ep[_qp] / _e0) - 1));
}
