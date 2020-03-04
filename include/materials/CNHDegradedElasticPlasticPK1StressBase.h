//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADDegradedElasticStressBase.h"

#define usingCNHDegradedElasticPlasticPK1StressMembers                                             \
  usingDegradedStressBaseMembers;                                                                  \
  using CNHDegradedElasticPlasticPK1StressBase<compute_stage>::updateDegradation;                  \
  using CNHDegradedElasticPlasticPK1StressBase<compute_stage>::updateIntermediateConfiguration;    \
  using CNHDegradedElasticPlasticPK1StressBase<compute_stage>::returnMapping;                      \
  using CNHDegradedElasticPlasticPK1StressBase<compute_stage>::updateCurrentConfiguration;         \
  using CNHDegradedElasticPlasticPK1StressBase<compute_stage>::enforceIsochoricity;                \
  using CNHDegradedElasticPlasticPK1StressBase<compute_stage>::computeFractureDrivingEnergy;       \
  using CNHDegradedElasticPlasticPK1StressBase<compute_stage>::dH_dep;                             \
  using CNHDegradedElasticPlasticPK1StressBase<compute_stage>::d2H_dep2;                           \
  using CNHDegradedElasticPlasticPK1StressBase<compute_stage>::_be_bar;                            \
  using CNHDegradedElasticPlasticPK1StressBase<compute_stage>::_G;                                 \
  using CNHDegradedElasticPlasticPK1StressBase<compute_stage>::_K;                                 \
  using CNHDegradedElasticPlasticPK1StressBase<compute_stage>::_J;                                 \
  using CNHDegradedElasticPlasticPK1StressBase<compute_stage>::_gq;                                \
  using CNHDegradedElasticPlasticPK1StressBase<compute_stage>::_ge;                                \
  using CNHDegradedElasticPlasticPK1StressBase<compute_stage>::_gp;                                \
  using CNHDegradedElasticPlasticPK1StressBase<compute_stage>::_W_pl;                              \
  using CNHDegradedElasticPlasticPK1StressBase<compute_stage>::_W_pl_degraded;                     \
  using CNHDegradedElasticPlasticPK1StressBase<compute_stage>::_E_el_degraded;

template <ComputeStage>
class CNHDegradedElasticPlasticPK1StressBase;

declareADValidParams(CNHDegradedElasticPlasticPK1StressBase);

template <ComputeStage compute_stage>
class CNHDegradedElasticPlasticPK1StressBase : public ADDegradedElasticStressBase<compute_stage>
{
public:
  static InputParameters validParams();

  CNHDegradedElasticPlasticPK1StressBase(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpStress() override;

  virtual void updateDegradation();
  virtual void updateIntermediateConfiguration();
  virtual void returnMapping();
  virtual void updateCurrentConfiguration();
  virtual void enforceIsochoricity();
  virtual void computeFractureDrivingEnergy();

  virtual ADReal H(ADReal ep) = 0;
  virtual ADReal dH_dep(ADReal ep) = 0;
  virtual ADReal d2H_dep2(ADReal ep) = 0;

  const ADMaterialProperty(RankTwoTensor) & _deformation_gradient;
  const MaterialProperty<RankTwoTensor> & _deformation_gradient_old;
  ADMaterialProperty(RankTwoTensor) & _be_bar;
  const MaterialProperty<RankTwoTensor> & _be_bar_old;
  ADMaterialProperty(Real) & _ep;
  const MaterialProperty<Real> & _ep_old;
  const ADVariableValue * _p;
  ADMaterialProperty(RankTwoTensor) & _plastic_strain;
  ADMaterialProperty(RankTwoTensor) & _cauchy_stress;
  const bool _legacy;
  const bool _isochoricity;
  const MaterialPropertyName _g_plastic_name;
  const ADMaterialProperty(Real) & _g_plastic;
  const MaterialPropertyName _W_pl_name;
  ADMaterialProperty(Real) & _W_pl;
  const MaterialProperty<Real> * _W_pl_old;
  ADMaterialProperty(Real) & _W_pl_degraded;
  ADMaterialProperty(Real) & _E_el_degraded;

  Real _G;
  Real _K;
  ADReal _gq;
  ADReal _ge;
  ADReal _gp;
  ADRankTwoTensor _f;
  ADRankTwoTensor _f_bar;
  ADReal _plastic_increment;
  ADRankTwoTensor _be_bar_trial;
  ADRankTwoTensor _s_trial;
  ADReal _s_trial_norm;
  ADRankTwoTensor _s;
  ADRankTwoTensor _np_trial;
  ADReal _J;
  ADRankTwoTensor _tau;
  ADRankTwoTensor _be_bar_dev;
  ADRankTwoTensor _be;
  ADRankTwoTensor _Cp;

  usingDegradedStressBaseMembers
};
