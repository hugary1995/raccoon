//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SmallStrainDegradedElasticPK2Stress_StrainVolDev.h"

registerADMooseObject("raccoonApp", SmallStrainDegradedElasticPK2Stress_StrainVolDev);

InputParameters
SmallStrainDegradedElasticPK2Stress_StrainVolDev::validParams()
{
  InputParameters params = ADDegradedElasticStressBase::validParams();
  params.addClassDescription("Compute degraded stress following small deformation elasticity "
                             "with a volumetric-deviatoric active/inactive split");
  return params;
}

SmallStrainDegradedElasticPK2Stress_StrainVolDev::SmallStrainDegradedElasticPK2Stress_StrainVolDev(
    const InputParameters & parameters)
  : ADDegradedElasticStressBase(parameters)
{
}

void
SmallStrainDegradedElasticPK2Stress_StrainVolDev::computeQpStress()
{
  // Isotropic elasticity is assumed and should be enforced
  const ADReal lambda = _elasticity_tensor[_qp](0, 0, 1, 1);
  const ADReal mu = _elasticity_tensor[_qp](0, 1, 0, 1);
  const ADReal K = lambda + 2.0 * mu / LIBMESH_DIM;

  // Identity tensor
  ADRankTwoTensor I2(RankTwoTensorTempl<ADReal>::initIdentity);

  // vol-dev decomposition
  ADRankTwoTensor E = _mechanical_strain[_qp];
  ADRankTwoTensor E_dev = E.deviatoric();
  ADReal trE = E.trace();
  ADReal trE_pos = Macaulay(trE);
  ADReal trE_neg = trE - trE_pos;

  // PK1 stress
  ADRankTwoTensor S0 = K * trE * I2 + 2.0 * mu * E_dev;
  ADRankTwoTensor S_neg = K * trE_neg * I2;
  _stress[_qp] = S_neg + g() * (S0 - S_neg);

  // elastic driving energy
  _E_el_active[_qp] = 0.5 * K * trE_pos * trE_pos + mu * E_dev.doubleContraction(E_dev);
}
