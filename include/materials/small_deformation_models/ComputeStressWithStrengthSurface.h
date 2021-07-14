//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "ADRankTwoTensorForward.h"
#include "ADSingleVariableReturnMappingSolution.h"
#include "BaseNameInterface.h"
#include "DerivativeMaterialPropertyNameInterface.h"

class ComputeStressWithStrengthSurface : public Material,
                                         public ADSingleVariableReturnMappingSolution,
                                         public BaseNameInterface,
                                         public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  ComputeStressWithStrengthSurface(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual ADRankTwoTensor computeStress(const ADRankTwoTensor & strain);
  virtual ADReal updateTrialState(const ADReal & delta_c);
  virtual void computeQpProperties() override;

  virtual ADReal computeResidual(const ADReal & effective_trial_stress,
                                 const ADReal & delta_c) override;

  virtual ADReal computeDerivative(const ADReal & effective_trial_stress,
                                   const ADReal & delta_c) override;

  virtual Real computeReferenceResidual(const ADReal & effective_trial_stress,
                                        const ADReal & delta_c) override;

  const ADMaterialProperty<Real> & _K;
  const ADMaterialProperty<Real> & _G;

  const ADMaterialProperty<RankTwoTensor> & _mechanical_strain;
  ADMaterialProperty<RankTwoTensor> & _stress;
  ADMaterialProperty<RankTwoTensor> & _elastic_strain;

  ADMaterialProperty<Real> & _c;
  const MaterialProperty<Real> & _c_old;
  ADMaterialProperty<RankTwoTensor> & _structured_strain;
  const MaterialProperty<RankTwoTensor> & _structured_strain_old;
  ADMaterialProperty<RankTwoTensor> & _N;

  const VariableName _d_name;
  const MaterialPropertyName _psin_name;
  ADMaterialProperty<Real> & _psin_active;
  const MaterialPropertyName _psie_name;
  ADMaterialProperty<Real> & _psie;
  ADMaterialProperty<Real> & _psie_active;
  const MaterialPropertyName _g_name;
  const ADMaterialProperty<Real> & _g;
  const ADMaterialProperty<Real> & _dg_dd;

  const Real _gamma_1;
  const Real _gamma_0;
};
