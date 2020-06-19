//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADDegradedElasticStressBase.h"
#include "MaterialPropertyUserObject.h"

class CNHDegradedElasticPlasticPK1StressBase : public ADDegradedElasticStressBase
{
public:
  static InputParameters validParams();

  CNHDegradedElasticPlasticPK1StressBase(const InputParameters & parameters);

protected:
  virtual ADReal gp();
  virtual void initQpStatefulProperties() override;
  virtual void computeQpStress() override;

  virtual void computeQpDegradation();
  virtual void updateIntermediateConfiguration();
  virtual void returnMapping();
  virtual void updateCurrentConfiguration();
  virtual void enforceIsochoricity();
  virtual void computeFractureDrivingEnergy();

  virtual ADReal H(ADReal ep) = 0;
  virtual ADReal dH_dep(ADReal ep) = 0;
  virtual ADReal d2H_dep2(ADReal ep) = 0;
  virtual ADReal plastic_dissipation(ADReal ep) = 0;

  const ADMaterialProperty<RankTwoTensor> & _deformation_gradient;
  const MaterialProperty<RankTwoTensor> & _deformation_gradient_old;
  const ADMaterialProperty<RankTwoTensor> & _deformation_gradient_increment;
  ADMaterialProperty<RankTwoTensor> & _be_bar;
  const MaterialProperty<RankTwoTensor> & _be_bar_old;
  ADMaterialProperty<Real> & _ep;
  const MaterialProperty<Real> & _ep_old;
  const ADVariableValue * _p;
  ADMaterialProperty<RankTwoTensor> & _plastic_strain;
  ADMaterialProperty<RankTwoTensor> & _cauchy_stress;
  ADMaterialProperty<RankTwoTensor> & _pk1_stress;
  const bool _legacy;
  const bool _isochoricity;
  const bool _use_cauchy_stress;
  const ADMaterialProperty<Real> * _g_plastic_mat;
  const ADMaterialPropertyUserObject * _g_plastic_uo;
  const MaterialPropertyName _W_pl_name;
  ADMaterialProperty<Real> & _W_pl;
  const MaterialProperty<Real> * _W_pl_old;
  ADMaterialProperty<Real> & _W_pl_degraded;
  ADMaterialProperty<Real> & _E_el_degraded;

  ADReal _G;
  ADReal _K;
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
};
