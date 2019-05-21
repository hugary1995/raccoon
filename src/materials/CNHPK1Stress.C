#include "CNHPK1Stress.h"

registerADMooseObject("raccoonApp", CNHPK1Stress);

defineADValidParams(
    CNHPK1Stress,
    ADComputeStressBase,
    params.addClassDescription("Compute stress using elasticity for small strains"););

template <ComputeStage compute_stage>
CNHPK1Stress<compute_stage>::CNHPK1Stress(const InputParameters & parameters)
  : ADComputeStressBase<compute_stage>(parameters),
    _elasticity_tensor(adGetADMaterialProperty<RankFourTensor>(_base_name + "elasticity_tensor")),
    _F(adGetADMaterialProperty<RankTwoTensor>(_base_name + "deformation_gradient"))
{
}

template <ComputeStage compute_stage>
void
CNHPK1Stress<compute_stage>::computeQpStress()
{
  // Isotropic elasticity is assumed and should be enforced
  ADReal lambda = _elasticity_tensor[_qp](0, 0, 1, 1);
  ADReal mu = _elasticity_tensor[_qp](0, 1, 0, 1);
  ADReal K = lambda + 2.0 * mu / 3.0;

  ADRankTwoTensor C = _mechanical_strain[_qp];
  ADRankTwoTensor F = _F[_qp];
  ADReal J = F.det();

  // U = 0.5 * K (0.5 * (J^2 - 1) - lnJ)
  // dU_dJ = 0.5 * K (J - 1/J)
  ADReal dU_dJ = 0.5 * K * (J - 1.0 / J);

  ADRankTwoTensor I2;
  I2.addIa(1.0);
  ADRankTwoTensor S =
      J * dU_dJ * C.inverse() + mu * std::pow(J, -2.0 / 3.0) * (I2 - C.trace() * C.inverse() / 3);

  // PK1 stress
  _stress[_qp] = F * S;
}
