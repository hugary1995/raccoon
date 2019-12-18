//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernel.h"
#include "DerivativeMaterialPropertyNameInterface.h"

#define usingPhaseFieldFractureEvolutionBaseMembers                                                \
  usingKernelMembers;                                                                              \
  using PhaseFieldFractureEvolutionBase<compute_stage>::_kappa;                                    \
  using PhaseFieldFractureEvolutionBase<compute_stage>::_M;                                        \
  using PhaseFieldFractureEvolutionBase<compute_stage>::_dg_dd;                                    \
  using PhaseFieldFractureEvolutionBase<compute_stage>::_dw_dd;                                    \
  using PhaseFieldFractureEvolutionBase<compute_stage>::fractureDrivingEnergy;

// Forward Declarations
template <ComputeStage>
class PhaseFieldFractureEvolutionBase;

declareADValidParams(PhaseFieldFractureEvolutionBase);

template <ComputeStage compute_stage>
class PhaseFieldFractureEvolutionBase : public ADKernel<compute_stage>,
                                        public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  PhaseFieldFractureEvolutionBase(const InputParameters & parameters);

protected:
  virtual ADReal fractureDrivingEnergy();

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
  const ADVariableValue * _D_var;

  usingKernelMembers;
};
