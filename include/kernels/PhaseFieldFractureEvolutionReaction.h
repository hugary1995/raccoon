//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"
#include "DerivativeMaterialPropertyNameInterface.h"
#include "FPIMaterialPropertyUserObject.h"

class PhaseFieldFractureEvolutionReaction : public ADKernelValue,
                                            public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  PhaseFieldFractureEvolutionReaction(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  /// name of the degradation function
  const ADMaterialProperty<Real> & _dg_dd;
  const bool _lag;
  const ADMaterialProperty<Real> * _D_mat;
  const MaterialProperty<Real> * _D_mat_old;
  const ADVariableValue * _D_var;
  const VariableValue * _D_var_old;
  const ADFPIMaterialPropertyUserObject * _D_uo;
};
