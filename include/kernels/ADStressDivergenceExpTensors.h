//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADStressDivergenceTensors.h"

template <ComputeStage>
class ADStressDivergenceExpTensors;

declareADValidParams(ADStressDivergenceTensors);

template <ComputeStage compute_stage>
class ADStressDivergenceExpTensors : public ADStressDivergenceTensors<compute_stage>
{
public:
  static InputParameters validParams();

  ADStressDivergenceExpTensors(const InputParameters & parameters);

protected:
  const MaterialProperty<RankTwoTensor> & _stress_old;

  virtual ADReal computeQpResidual() override;

  usingStressDivergenceTensorsMembers;
};
