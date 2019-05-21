//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SmallStrainDegradedPK2Stress_NoSplit.h"

registerADMooseObject("raccoonApp", SmallStrainDegradedPK2Stress_NoSplit);

defineADValidParams(
    SmallStrainDegradedPK2Stress_NoSplit,
    ADDegradedStressBase,
    params.addClassDescription("Compute degraded stress following small deformation elasticity "
                               "without active/inactive split"););

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
  ADReal E_el_pos = 0.5 * S.doubleContraction(E);

  // enforce irreversibility using history approach
  if (_E_el_pos_old)
  {
    E_el_pos = std::max(E_el_pos, (*_E_el_pos_old)[_qp]);
    E_el_pos = std::max(E_el_pos, _b[_qp]);
  }

  // store the positive elastic energy so that it becomes the old value in the next step
  _E_el_pos[_qp] = E_el_pos;
}
