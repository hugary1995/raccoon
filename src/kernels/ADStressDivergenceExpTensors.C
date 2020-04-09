//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADStressDivergenceExpTensors.h"

registerADMooseObject("raccoonApp", ADStressDivergenceExpTensors);

defineADLegacyParams(ADStressDivergenceExpTensors);

template <ComputeStage compute_stage>
InputParameters
ADStressDivergenceExpTensors<compute_stage>::validParams()
{
  InputParameters params = ADStressDivergenceTensors<compute_stage>::validParams();
  return params;
}

template <ComputeStage compute_stage>
ADStressDivergenceExpTensors<compute_stage>::ADStressDivergenceExpTensors(
    const InputParameters & parameters)
  : ADStressDivergenceTensors<compute_stage>(parameters),
    _stress_old(getMaterialPropertyOldByName<RankTwoTensor>(_base_name + "stress"))
{
}

template <ComputeStage compute_stage>
ADReal
ADStressDivergenceExpTensors<compute_stage>::computeQpResidual()
{
  return _stress_old[_qp].row(_component) * _grad_test[_i][_qp];
}
