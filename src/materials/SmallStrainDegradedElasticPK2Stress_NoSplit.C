//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SmallStrainDegradedElasticPK2Stress_NoSplit.h"

registerADMooseObject("raccoonApp", SmallStrainDegradedElasticPK2Stress_NoSplit);

InputParameters
SmallStrainDegradedElasticPK2Stress_NoSplit::validParams()
{
  InputParameters params = ADDegradedElasticStressBase::validParams();
  params.addClassDescription("Compute degraded stress following small deformation elasticity "
                             "without active/inactive split");
  return params;
}

SmallStrainDegradedElasticPK2Stress_NoSplit::SmallStrainDegradedElasticPK2Stress_NoSplit(
    const InputParameters & parameters)
  : ADDegradedElasticStressBase(parameters)
{
}

void
SmallStrainDegradedElasticPK2Stress_NoSplit::computeQpStress()
{
  ADRankTwoTensor E = _mechanical_strain[_qp];

  // PK2 stress
  ADRankTwoTensor S = _elasticity_tensor[_qp] * E;
  _stress[_qp] = g() * S;

  // elastic driving energy
  _E_el_active[_qp] = 0.5 * S.doubleContraction(E);
}
