//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"

// Forward Declarations
template <ComputeStage>
class PhaseFieldFractureContactKKT;

declareADValidParams(PhaseFieldFractureContactKKT);

template <ComputeStage compute_stage>
class PhaseFieldFractureContactKKT : public ADKernelValue<compute_stage>
{
public:
  static InputParameters validParams();

  PhaseFieldFractureContactKKT(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  const ADMaterialProperty(Real) & _penetration;

  usingKernelValueMembers;
};
