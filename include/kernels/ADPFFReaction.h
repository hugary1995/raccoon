//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"
#include "DerivativeMaterialPropertyNameInterface.h"
#include "FPIMaterialPropertyUserObject.h"

class ADPFFReaction : public ADKernelValue, public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  ADPFFReaction(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  /// name of the degradation function
  const ADMaterialProperty<Real> & _dg_dd;
  const bool _lag;
  const ADMaterialProperty<Real> * _D_mat;
  const MaterialProperty<Real> * _D_mat_old;
  const ADVariableValue * _D_var;
  const VariableValue * _D_var_old;
  const ADMaterialPropertyUserObject * _D_uo;
};
