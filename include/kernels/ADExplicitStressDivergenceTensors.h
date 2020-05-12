//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADStressDivergenceTensors.h"

class ADExplicitStressDivergenceTensors : public ADStressDivergenceTensors
{
public:
  static InputParameters validParams();

  ADExplicitStressDivergenceTensors(const InputParameters & parameters);

protected:
  const MaterialProperty<RankTwoTensor> & _stress_old;

  virtual ADReal computeQpResidual() override;
};
