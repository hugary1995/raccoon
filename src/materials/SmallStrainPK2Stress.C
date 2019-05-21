#include "SmallStrainPK2Stress.h"

registerADMooseObject("raccoonApp", SmallStrainPK2Stress);

defineADValidParams(
    SmallStrainPK2Stress,
    ADComputeStressBase,
    params.addClassDescription("Compute stress following small deformation elasticity"););

template <ComputeStage compute_stage>
SmallStrainPK2Stress<compute_stage>::SmallStrainPK2Stress(const InputParameters & parameters)
  : ADComputeStressBase<compute_stage>(parameters),
    _elasticity_tensor(adGetADMaterialProperty<RankFourTensor>(_base_name + "elasticity_tensor"))
{
}

template <ComputeStage compute_stage>
void
SmallStrainPK2Stress<compute_stage>::computeQpStress()
{
  // Isotropic elasticity is assumed and should be enforced
  _stress[_qp] = _elasticity_tensor[_qp] * _mechanical_strain[_qp];

  // elastic strain in reference configuration
  _elastic_strain[_qp] = _mechanical_strain[_qp];
}
