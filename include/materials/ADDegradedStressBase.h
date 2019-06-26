//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADComputeStressBase.h"

#define usingDegradedStressBaseMembers                                                             \
  usingComputeStressBaseMembers;                                                                   \
  using ADDegradedStressBase<compute_stage>::computeQpHistory;                                     \
  using ADDegradedStressBase<compute_stage>::computeQpTractionFreeStress;                          \
  using ADDegradedStressBase<compute_stage>::Macaulay;                                             \
  using ADDegradedStressBase<compute_stage>::dMacaulay;                                            \
  using ADDegradedStressBase<compute_stage>::_elasticity_tensor;                                   \
  using ADDegradedStressBase<compute_stage>::_d;                                                   \
  using ADDegradedStressBase<compute_stage>::_grad_d;                                              \
  using ADDegradedStressBase<compute_stage>::_d_crit;                                              \
  using ADDegradedStressBase<compute_stage>::_g_name;                                              \
  using ADDegradedStressBase<compute_stage>::_g;                                                   \
  using ADDegradedStressBase<compute_stage>::_history;                                             \
  using ADDegradedStressBase<compute_stage>::_E_el_name;                                           \
  using ADDegradedStressBase<compute_stage>::_E_el_pos;                                            \
  using ADDegradedStressBase<compute_stage>::_E_el_pos_old;                                        \
  using ADDegradedStressBase<compute_stage>::_b_name;                                              \
  using ADDegradedStressBase<compute_stage>::_b;

template <ComputeStage>
class ADDegradedStressBase;

declareADValidParams(ADDegradedStressBase);

template <ComputeStage compute_stage>
class ADDegradedStressBase : public ADComputeStressBase<compute_stage>
{
public:
  ADDegradedStressBase(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;
  virtual void initQpStatefulProperties() override;
  virtual void computeQpStress() = 0;

  /// initialize stateful elastic driving energy
  virtual ADReal computeQpHistory(const ADReal & E_el_pos);

  /// enforce traction-free boundary condition
  virtual void computeQpTractionFreeStress();

  /// Macaulay bracket operator
  virtual ADReal Macaulay(ADReal x);

  /// derivative of Macaulay bracket with respect to its operand
  virtual ADReal dMacaulay(ADReal x);

  /// Elasticity tensor material property
  const ADMaterialProperty(RankFourTensor) & _elasticity_tensor;

  /// damage value
  const ADVariableValue & _d;

  /// damage gradient
  const ADVariableGradient & _grad_d;

  /// critical damage value for enforcing the traction-free boundary condition
  const Real _d_crit;

  /// degradation name
  const MaterialPropertyName _g_name;

  /// degradation
  const ADMaterialProperty(Real) & _g;

  /// whether to enforce irreversibility using history
  const bool _history;

  /// elastic energy name
  const MaterialPropertyName _E_el_name;

  /// positive elastic driving energy
  ADMaterialProperty(Real) & _E_el_pos;

  /// old value of the driving energy
  const MaterialProperty<Real> * _E_el_pos_old;

  /// degradation name
  const MaterialPropertyName _b_name;

  /// degradation
  const ADMaterialProperty(Real) & _b;

  usingComputeStressBaseMembers;
};
