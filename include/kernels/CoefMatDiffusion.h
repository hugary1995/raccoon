//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelGrad.h"

// Forward Declarations
template <ComputeStage>
class CoefMatDiffusion;

declareADValidParams(CoefMatDiffusion);

template <ComputeStage compute_stage>
class CoefMatDiffusion : public ADKernelGrad<compute_stage>
{
public:
  static InputParameters validParams();

  CoefMatDiffusion(const InputParameters & parameters);

protected:
  virtual ADRealVectorValue precomputeQpResidual() override;

  const Real _coef;

  const ADMaterialProperty(Real) * _prop;

  usingKernelGradMembers;
};
