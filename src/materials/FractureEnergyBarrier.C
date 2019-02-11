#include "FractureEnergyBarrier.h"

registerMooseObject("raccoonApp", FractureEnergyBarrier);

template <>
InputParameters
validParams<FractureEnergyBarrier>()
{
  InputParameters params = validParams<Material>();
  params.addClassDescription("Compute the critical fracture energy given degradation function, "
                             "local disiipation and mobility");
  params.addRequiredCoupledVar("d", "damage variable that these material paramters apply to");
  params.addRequiredParam<Real>("initial_local_dissipation_slope", "value of dw_dd at d = 0");
  params.addRequiredParam<Real>("initial_degradation_slope", "value of dg_dd at d = 0");
  params.addParam<MaterialPropertyName>(
      "mob_name", "M", "name of the material that holds the mobility");
  params.addParam<MaterialPropertyName>(
      "fracture_energy_barrier_name", "b", "name of the fracture energy barrier");

  return params;
}

FractureEnergyBarrier::FractureEnergyBarrier(const InputParameters & parameters)
  : Material(parameters),
    _var_name(getVar("d", 0)->name()),
    _dw_dd(getParam<Real>("initial_local_dissipation_slope")),
    _dg_dd(getParam<Real>("initial_degradation_slope")),
    _M(getMaterialPropertyByName<Real>(getParam<MaterialPropertyName>("mob_name") + "_" +
                                       _var_name)),
    _E_crit(declareProperty<Real>(getParam<MaterialPropertyName>("fracture_energy_barrier_name") +
                                  "_" + _var_name))
{
  if (_dg_dd == 0.0)
    mooseError("Initial slope of the degradation function cannot be zero");
}

void
FractureEnergyBarrier::computeQpProperties()
{
  _E_crit[_qp] = -_M[_qp] * _dw_dd / _dg_dd;
}
