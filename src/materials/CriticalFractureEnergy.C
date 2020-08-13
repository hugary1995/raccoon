//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CriticalFractureEnergy.h"

registerMooseObject("raccoonApp", CriticalFractureEnergy);
registerMooseObject("raccoonApp", ADCriticalFractureEnergy);

template <bool is_ad>
InputParameters
CriticalFractureEnergyTempl<is_ad>::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("Compute the critical fracture energy given degradation function, "
                             "local disiipation and mobility");
  params.addRequiredParam<FunctionName>("initial_local_dissipation_slope",
                                        "value of dw_dd at d = 0");
  params.addRequiredParam<FunctionName>("initial_degradation_slope", "value of dg_dd at d = 0");
  params.addParam<MaterialPropertyName>(
      "mobility_name", "mobility", "name of the material that holds the mobility");
  params.addParam<MaterialPropertyName>("critical_fracture_energy_name",
                                        "critical_fracture_energy",
                                        "name of the fracture energy barrier");

  return params;
}
template <bool is_ad>
CriticalFractureEnergyTempl<is_ad>::CriticalFractureEnergyTempl(const InputParameters & parameters)
  : Material(parameters),
    _dw_dd(getFunction("initial_local_dissipation_slope")),
    _dg_dd(getFunction("initial_degradation_slope")),
    _M(getGenericMaterialProperty<Real, is_ad>(getParam<MaterialPropertyName>("mobility_name"))),
    _psi_critical(declareGenericProperty<Real, is_ad>(
        getParam<MaterialPropertyName>("critical_fracture_energy_name")))
{
}

template <bool is_ad>
void
CriticalFractureEnergyTempl<is_ad>::computeQpProperties()
{
  Real dw_dd = _dw_dd.value(0.0, _q_point[_qp]);
  Real dg_dd = _dg_dd.value(0.0, _q_point[_qp]);

  if (dg_dd == 0.0)
    mooseError("Initial slope of the degradation function cannot be zero");

  _psi_critical[_qp] = -_M[_qp] * dw_dd / dg_dd;
}

template class CriticalFractureEnergyTempl<false>;
template class CriticalFractureEnergyTempl<true>;
