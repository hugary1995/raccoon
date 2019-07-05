//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SmallStrainDegradedPK2Stress_StrainVolDev.h"

registerADMooseObject("raccoonApp", SmallStrainDegradedPK2Stress_StrainVolDev);

defineADValidParams(
    SmallStrainDegradedPK2Stress_StrainVolDev,
    ADDegradedStressBase,
    params.addClassDescription("Compute degraded stress following small deformation elasticity "
                               "with a volumetric-deviatoric active/inactive split"););

template <ComputeStage compute_stage>
SmallStrainDegradedPK2Stress_StrainVolDev<compute_stage>::SmallStrainDegradedPK2Stress_StrainVolDev(
    const InputParameters & parameters)
  : ADDegradedStressBase<compute_stage>(parameters)
{
}

template <ComputeStage compute_stage>
void
SmallStrainDegradedPK2Stress_StrainVolDev<compute_stage>::computeQpStress()
{
  // Isotropic elasticity is assumed and should be enforced
  const Real lambda = _elasticity_tensor[_qp](0, 0, 1, 1);
  const Real mu = _elasticity_tensor[_qp](0, 1, 0, 1);
  const Real K = lambda + 2.0 * mu / LIBMESH_DIM;

  // vol-dev decomposition
  ADRankTwoTensor E = _mechanical_strain[_qp];
  ADRankTwoTensor E_dev = E.deviatoric();
  ADReal trE = E.trace();
  ADReal trE_pos = Macaulay(trE);
  ADReal trE_neg = trE - trE_pos;

  // Identity tensor
  ADRankTwoTensor I2;
  I2.addIa(1.0);

  // PK1 stress
  ADRankTwoTensor S0 = K * trE * I2 + 2.0 * mu * E_dev;
  ADRankTwoTensor S_neg = K * trE_neg * I2;
  _stress[_qp] = S_neg + _g[_qp] * (S0 - S_neg);

  // elastic driving energy
  ADReal E_el_pos = 0.5 * K * trE_pos * trE_pos + mu * E_dev.doubleContraction(E_dev);

  // enforce irreversibility using history approach
  if (_E_el_pos_old)
    E_el_pos = computeQpHistory(E_el_pos);

  // store the positive elastic energy
  // note that it becomes the old value in the next step
  _E_el_pos[_qp] = E_el_pos;
}
