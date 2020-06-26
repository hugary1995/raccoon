//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SmallStrainDegradedElasticPK2Stress_StrainSpectral.h"

registerADMooseObject("raccoonApp", SmallStrainDegradedElasticPK2Stress_StrainSpectral);

InputParameters
SmallStrainDegradedElasticPK2Stress_StrainSpectral::validParams()
{
  InputParameters params = ADDegradedElasticStressBase::validParams();
  params.addClassDescription("Compute degraded stress following small deformation elasticity "
                             "with a spectra-based active/inactive split");
  return params;
}

SmallStrainDegradedElasticPK2Stress_StrainSpectral::
    SmallStrainDegradedElasticPK2Stress_StrainSpectral(const InputParameters & parameters)
  : ADDegradedElasticStressBase(parameters)
{
}

void
SmallStrainDegradedElasticPK2Stress_StrainSpectral::computeQpStress()
{
  // Isotropic elasticity is assumed and should be enforced
  const ADReal lambda = _elasticity_tensor[_qp](0, 0, 1, 1);
  const ADReal mu = _elasticity_tensor[_qp](0, 1, 0, 1);

  // Identity tensor
  ADRankTwoTensor I2(RankTwoTensorTempl<ADReal>::initIdentity);

  // spectral decomposition
  ADRankTwoTensor E = _mechanical_strain[_qp];
  ADRankTwoTensor Q;
  std::vector<ADReal> d;
  E.symmetricEigenvaluesEigenvectors(d, Q);

  // positive part
  D_pos.fillFromInputVector(Macaulay(d));
  ADRankTwoTensor E_pos = Q * D_pos * Q.transpose();

  ADReal trE = E.trace();
  ADReal trE_pos = Macaulay(trE);

  // PK1 stress
  ADRankTwoTensor S0 = lambda * trE * I2 + 2.0 * mu * E;
  ADRankTwoTensor S_pos = lambda * trE_pos * I2 + 2.0 * mu * E_pos;
  _stress[_qp] = S0 - (1.0 - g()) * S_pos;

  // elastic driving energy
  _E_el_active[_qp] = 0.5 * lambda * trE_pos * trE_pos + mu * E_pos.doubleContraction(E_pos);
}
