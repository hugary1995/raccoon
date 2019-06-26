//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SmallStrainDegradedPK2Stress_StrainSpectral.h"

registerADMooseObject("raccoonApp", SmallStrainDegradedPK2Stress_StrainSpectral);

defineADValidParams(
    SmallStrainDegradedPK2Stress_StrainSpectral,
    ADDegradedStressBase,
    params.addClassDescription("Compute degraded stress following small deformation elasticity "
                               "without active/inactive split"););

template <ComputeStage compute_stage>
SmallStrainDegradedPK2Stress_StrainSpectral<
    compute_stage>::SmallStrainDegradedPK2Stress_StrainSpectral(const InputParameters & parameters)
  : ADDegradedStressBase<compute_stage>(parameters)
{
}

template <ComputeStage compute_stage>
void
SmallStrainDegradedPK2Stress_StrainSpectral<compute_stage>::computeQpStress()
{
  // Isotropic elasticity is assumed and should be enforced
  ADReal lambda = _elasticity_tensor[_qp](0, 0, 1, 1);
  ADReal mu = _elasticity_tensor[_qp](0, 1, 0, 1);

  // spectral decomposition
  ADRankTwoTensor E = _mechanical_strain[_qp];
  ADRankTwoTensor Q;
  std::vector<ADReal> d;
  E.symmetricEigenvaluesEigenvectors(d, Q);

  // separate into positive and negative parts
  ADRankTwoTensor D_pos;
  for (unsigned int i = 0; i < LIBMESH_DIM; i++)
    D_pos(i, i) = Macaulay(d[i]);
  ADRankTwoTensor E_pos = Q * D_pos * Q.transpose();

  ADReal trE = E.trace();
  ADReal trE_pos = Macaulay(trE);

  // Identity tensor
  ADRankTwoTensor I2;
  I2.addIa(1.0);

  // PK1 stress
  ADRankTwoTensor S0 = lambda * trE * I2 + 2.0 * mu * E;
  ADRankTwoTensor S_pos = lambda * trE_pos * I2 + 2.0 * mu * E_pos;
  _stress[_qp] = S0 - (1.0 - _g[_qp]) * S_pos;

  // elastic driving energy
  ADReal E_el_pos = 0.5 * lambda * trE_pos * trE_pos + mu * E_pos.doubleContraction(E_pos);

  // enforce irreversibility using history approach
  if (_E_el_pos_old)
    E_el_pos = computeQpHistory(E_el_pos);

  // store the positive elastic energy
  // note that it becomes the old value in the next step
  _E_el_pos[_qp] = E_el_pos;
}
