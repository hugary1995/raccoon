//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernel.h"
#include "DerivativeMaterialPropertyNameInterface.h"

// Forward Declarations
template <ComputeStage>
class PhaseFieldFractureEvolution;

declareADValidParams(PhaseFieldFractureEvolution);

template <ComputeStage compute_stage>
class PhaseFieldFractureEvolution : public ADKernel<compute_stage>,
                                    public DerivativeMaterialPropertyNameInterface
{
public:
  PhaseFieldFractureEvolution(const InputParameters & parameters);

protected:
  virtual ADResidual computeQpResidual() override;

  const MaterialProperty<Real> & _kappa;
  const MaterialProperty<Real> & _M;
  const MaterialPropertyName _g_name;
  const ADMaterialProperty(Real) & _dg_dd;
  const MaterialPropertyName _w_name;
  const ADMaterialProperty(Real) & _dw_dd;
  const bool _lag;
  const bool _D_material_coupled;
  const bool _D_variable_coupled;
  const ADMaterialProperty(Real) * _D_mat;
  const MaterialProperty<Real> * _D_mat_old;
  const VariableValue * _D_var;

  usingKernelMembers;
};
