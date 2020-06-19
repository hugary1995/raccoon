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
  params.addParam<MaterialPropertyName>("degradation_mat", "name of the degradation function");
  params.addParam<UserObjectName>("degradation_uo",
                                  "name of the userobject that holds the degradation");
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
    _g_mat(isParamValid("degradation_mat") ? &getADMaterialProperty<Real>("degradation_mat")
                                           : nullptr),
    _g_uo(isParamValid("degradation_uo")
              ? &getUserObject<ADMaterialPropertyUserObject>("degradation_uo")
              : nullptr),
    _w(getADMaterialProperty<Real>("local_dissipation_name")),
    _ep(getADMaterialProperty<Real>(_base_name + "effective_plastic_strain"))
{
  bool provided_by_mat = _g_mat;
  bool provided_by_uo = _g_uo;

  /// degradation should be provided
  if (!provided_by_mat && !provided_by_uo)
    mooseError("no degradation provided.");

  /// degradation should not be multiply defined
  if ((provided_by_mat ? 1 : 0) + (provided_by_uo ? 1 : 0) > 1)
    mooseError("degradation multiply defined.");
}

ADReal
CoalescenceEnergy::g()
{
  if (_g_mat)
    return (*_g_mat)[_qp];
  else if (_g_uo)
    return _g_uo->getData(_current_elem, _qp);
  else
    mooseError("Internal Error");

  return 0;
}

void
CoalescenceEnergy::initQpStatefulProperties()
{
  computeQpProperties();
}

void
CoalescenceEnergy::computeQpProperties()
{
  _M[_qp] = g() * _c * _M0[_qp] * _ep[_qp];
  _E_coalesce[_qp] = _c * _M0[_qp] * _ep[_qp] * _w[_qp];
}
