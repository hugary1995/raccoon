//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADPFFDiffusion.h"
#include "SolutionUserObject.h"

class ADPFFDiffusionRecover : public ADPFFDiffusion
{
public:
  static InputParameters validParams();

  ADPFFDiffusionRecover(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  const ADMaterialProperty<RankTwoTensor> & _Fbarprev;
};
