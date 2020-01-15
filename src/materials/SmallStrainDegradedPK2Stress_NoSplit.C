//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SmallStrainDegradedPK2Stress_NoSplit.h"

registerADMooseObject("raccoonApp", SmallStrainDegradedPK2Stress_NoSplit);

defineADLegacyParams(SmallStrainDegradedPK2Stress_NoSplit);

template <ComputeStage compute_stage>
InputParameters
SmallStrainDegradedPK2Stress_NoSplit<compute_stage>::validParams()
{
  InputParameters params = ADDegradedStressBase<compute_stage>::validParams();
  params.addClassDescription("Compute degraded stress following small deformation elasticity "
                             "without active/inactive split");
  return params;
}

template <ComputeStage compute_stage>
SmallStrainDegradedPK2Stress_NoSplit<compute_stage>::SmallStrainDegradedPK2Stress_NoSplit(
    const InputParameters & parameters)
  : ADDegradedStressBase<compute_stage>(parameters)
{
}

template <ComputeStage compute_stage>
void
SmallStrainDegradedPK2Stress_NoSplit<compute_stage>::computeQpStress()
{
  ADRankTwoTensor E = _mechanical_strain[_qp];

  // PK2 stress
  ADRankTwoTensor S = _elasticity_tensor[_qp] * E;
  _stress[_qp] = _g[_qp] * S;

  // elastic driving energy
  _E_el_active[_qp] = 0.5 * S.doubleContraction(E);
}
