#include "FractureEnergyBarrier.h"

registerMooseObject("raccoonApp", FractureEnergyBarrier);

template <>
InputParameters
validParams<FractureEnergyBarrier>()
{
  InputParameters params = validParams<Material>();
  params.addClassDescription("Compute the critical fracture energy given degradation function, "
                             "local disiipation and mobility");
  params.addRequiredParam<FunctionName>("initial_local_dissipation_slope",
                                        "value of dw_dd at d = 0");
  params.addRequiredParam<FunctionName>("initial_degradation_slope", "value of dg_dd at d = 0");
  params.addParam<MaterialPropertyName>(
      "mob_name", "M", "name of the material that holds the mobility");
  params.addParam<MaterialPropertyName>(
      "fracture_energy_barrier_name", "b", "name of the fracture energy barrier");

  return params;
}

FractureEnergyBarrier::FractureEnergyBarrier(const InputParameters & parameters)
  : Material(parameters),
    _dw_dd(getFunction("initial_local_dissipation_slope")),
    _dg_dd(getFunction("initial_degradation_slope")),
    _M(getMaterialPropertyByName<Real>(getParam<MaterialPropertyName>("mob_name"))),
    _b(declareProperty<Real>(getParam<MaterialPropertyName>("fracture_energy_barrier_name")))
{
}

void
FractureEnergyBarrier::computeQpProperties()
{
  Real dw_dd = _dw_dd.value(0.0, _q_point[_qp]);
  Real dg_dd = _dg_dd.value(0.0, _q_point[_qp]);

  if (dg_dd == 0.0)
    mooseError("Initial slope of the degradation function cannot be zero");

  _b[_qp] = -_M[_qp] * dw_dd / dg_dd;
}
