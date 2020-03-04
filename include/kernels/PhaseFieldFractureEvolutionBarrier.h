//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"
#include "DerivativeMaterialPropertyNameInterface.h"

// Forward Declarations
template <ComputeStage>
class PhaseFieldFractureEvolutionBarrier;

declareADValidParams(PhaseFieldFractureEvolutionBarrier);

template <ComputeStage compute_stage>
class PhaseFieldFractureEvolutionBarrier : public ADKernelValue<compute_stage>,
                                           public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  PhaseFieldFractureEvolutionBarrier(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  const MaterialProperty<Real> & _M;

  const ADMaterialProperty(Real) & _dw_dd;

  usingKernelValueMembers;
};
