//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"
#include "DerivativeMaterialPropertyNameInterface.h"
#include "MaterialPropertyUserObject.h"

// Forward Declarations
template <ComputeStage>
class PhaseFieldFractureEvolutionReaction;

declareADValidParams(PhaseFieldFractureEvolutionReaction);

template <ComputeStage compute_stage>
class PhaseFieldFractureEvolutionReaction : public ADKernelValue<compute_stage>,
                                            public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  PhaseFieldFractureEvolutionReaction(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  /// Allen-Cahn mobility calculated from fracture properties
  const MaterialProperty<Real> & _M;

  /// name of the degradation function
  const ADMaterialProperty(Real) & _dg_dd;
  const ADMaterialProperty(Real) & _dw_dd;
  const bool _lag;
  const ADMaterialProperty(Real) * _D_mat;
  const MaterialProperty<Real> * _D_mat_old;
  const ADVariableValue * _D_var;
  const VariableValue * _D_var_old;
  const MaterialPropertyUserObject * _D_uo;

  usingKernelValueMembers;
};
