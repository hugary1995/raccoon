#include "ADDegradedStressBase.h"

defineADValidParams(
    ADDegradedStressBase,
    ADComputeStressBase,
    params.addClassDescription("Base class for computing damage degraded stress");
    params.addParam<MaterialPropertyName>("elastic_energy_name",
                                          "E_el",
                                          "name of the material that holds the elastic energy");
    params.addParam<MaterialPropertyName>(
        "fracture_energy_barrier_name",
        "b",
        "name of the material that holds the fracture energy barrier");
    params.addParam<MaterialPropertyName>("degradation_name",
                                          "g",
                                          "name of the material that holds the degradation");
    params.addParam<bool>("history",
                          true,
                          "whether irreversibility is enforced using a history approach"););

template <ComputeStage compute_stage>
ADDegradedStressBase<compute_stage>::ADDegradedStressBase(const InputParameters & parameters)
  : ADComputeStressBase<compute_stage>(parameters),
    _elasticity_tensor(adGetADMaterialProperty<RankFourTensor>(_base_name + "elasticity_tensor")),
    _g_name(adGetParam<MaterialPropertyName>("degradation_name")),
    _g(adGetADMaterialProperty<Real>(_g_name)),
    _history(adGetParam<bool>("history")),
    _E_el_name(adGetParam<MaterialPropertyName>("elastic_energy_name")),
    _E_el_pos(adDeclareADProperty<Real>(_E_el_name)),
    _E_el_pos_old(_history ? &adGetMaterialPropertyOld<Real>(_E_el_name) : NULL),
    _b_name(adGetParam<MaterialPropertyName>("fracture_energy_barrier_name")),
    _b(adGetADMaterialProperty<Real>(_b_name))
{
}

template <ComputeStage compute_stage>
void
ADDegradedStressBase<compute_stage>::initQpStatefulProperties()
{
  // enforce irreversibility using history approach
  if (_E_el_pos_old)
    _E_el_pos[_qp] = _b[_qp];
}

template <ComputeStage compute_stage>
ADReal
ADDegradedStressBase<compute_stage>::Macaulay(ADReal x)
{
  return 0.5 * (x + std::abs(x));
}

template <ComputeStage compute_stage>
ADReal
ADDegradedStressBase<compute_stage>::dMacaulay(ADReal x)
{
  ADReal val = x > 0.0 ? 1.0 : 0.0;
  return val;
}

adBaseClass(ADDegradedStressBase);
