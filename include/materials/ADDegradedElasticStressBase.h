//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADComputeStressBase.h"

#define usingDegradedStressBaseMembers                                                             \
  usingComputeStressBaseMembers;                                                                   \
  using ADDegradedElasticStressBase<compute_stage>::computeQpTractionFreeStress;                          \
  using ADDegradedElasticStressBase<compute_stage>::Macaulay;                                             \
  using ADDegradedElasticStressBase<compute_stage>::_elasticity_tensor;                                   \
  using ADDegradedElasticStressBase<compute_stage>::_d;                                                   \
  using ADDegradedElasticStressBase<compute_stage>::_grad_d;                                              \
  using ADDegradedElasticStressBase<compute_stage>::_d_crit;                                              \
  using ADDegradedElasticStressBase<compute_stage>::_g_name;                                              \
  using ADDegradedElasticStressBase<compute_stage>::_g;                                                   \
  using ADDegradedElasticStressBase<compute_stage>::_E_el_name;                                           \
  using ADDegradedElasticStressBase<compute_stage>::_E_el_active;

template <ComputeStage>
class ADDegradedElasticStressBase;

declareADValidParams(ADDegradedElasticStressBase);

template <ComputeStage compute_stage>
class ADDegradedElasticStressBase : public ADComputeStressBase<compute_stage>
{
public:
  static InputParameters validParams();
  ADDegradedElasticStressBase(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;
  virtual void computeQpStress() override {}

  /// enforce traction-free boundary condition
  virtual void computeQpTractionFreeStress();

  ///{@
  /// Macaulay bracket operator
  virtual ADReal Macaulay(ADReal x);
  virtual std::vector<ADReal> Macaulay(std::vector<ADReal> x);
  ///@}

  /// Elasticity tensor material property
  const MaterialProperty<RankFourTensor> & _elasticity_tensor;

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

  /// elastic energy name
  const MaterialPropertyName _E_el_name;

  /// positive elastic driving energy
  ADMaterialProperty(Real) & _E_el_active;

  usingComputeStressBaseMembers;
};
