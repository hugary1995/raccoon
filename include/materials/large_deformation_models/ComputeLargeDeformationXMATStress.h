//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"
#include "ADSingleVariableReturnMappingSolution.h"
#include "DerivativeMaterialPropertyNameInterface.h"
#include "ADRankTwoTensorForward.h"

/**
 * ComputeLargeDeformationXMATStress computes the stress given certain hyperelasticity and
 * optionally a plasticity model and a viscoelasticity model
 */
class ComputeLargeDeformationXMATStress : public Material,
                                          public BaseNameInterface,
                                          public ADSingleVariableReturnMappingSolution,
                                          public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  ComputeLargeDeformationXMATStress(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;
  virtual void updateState();
  virtual ADRankTwoTensor MandelStress(const ADRankTwoTensor & Fe,
                                       const bool plasticity_update = false);
  virtual ADRankTwoTensor CauchyStress(const ADRankTwoTensor & Fe);
  virtual Real computeReferenceResidual(const ADReal & effective_trial_stress,
                                        const ADReal & delta_ep) override;
  virtual ADReal computeResidual(const ADReal & effective_trial_stress,
                                 const ADReal & delta_ep) override;
  virtual ADReal computeDerivative(const ADReal & effective_trial_stress,
                                   const ADReal & delta_ep) override;
  virtual ADReal plasticEnergy(const ADReal & ep, const unsigned int derivative = 0);

  // @{ The mechanical strain excluding eigen strains from the total strain
  const ADMaterialProperty<RankTwoTensor> & _Fm;
  // @}

  /// The stress
  ADMaterialProperty<RankTwoTensor> & _stress;

  /// The elastic strain
  ADMaterialProperty<RankTwoTensor> & _Fe;

  /// The bulk modulus
  const ADMaterialProperty<Real> & _K;

  /// The shear modulus
  const ADMaterialProperty<Real> & _G;

  /// Name of the phase-field variable
  const VariableName _d_name;

  // @{ Strain energy density and its derivative w/r/t damage
  const MaterialPropertyName _psie_name;
  ADMaterialProperty<Real> & _psie;
  ADMaterialProperty<Real> & _psie_active;
  ADMaterialProperty<Real> & _dpsie_dd;
  // @}

  // @{ The degradation function and its derivative w/r/t damage
  const MaterialPropertyName _g_name;
  const ADMaterialProperty<Real> & _g;
  const ADMaterialProperty<Real> & _dg_dd;
  // @}

  /// The plastic deformation gradient
  ADMaterialProperty<RankTwoTensor> & _Fp;
  const MaterialProperty<RankTwoTensor> & _Fp_old;

  /// The (scalar) effective plastic strain
  ADMaterialProperty<Real> & _ep;
  const MaterialProperty<Real> & _ep_old;

  /// The flow direction
  ADMaterialProperty<RankTwoTensor> & _Np;

  // @{ The parameters of the power law creep update
  const Real _coefficient;
  const Real _exponent;
  // @}

  // @{ The plastic energy parameters
  const ADMaterialProperty<Real> & _sigma_0;
  const ADMaterialProperty<Real> & _arrhenius_coef;
  const Real _eps;
  // @}

  // @{ Plastic energy density and its derivative w/r/t damage
  const MaterialPropertyName _psip_name;
  ADMaterialProperty<Real> & _psip;
  ADMaterialProperty<Real> & _psip_active;
  ADMaterialProperty<Real> & _dpsip_dd;
  // @}

  // @{ The degradation function and its derivative w/r/t damage
  const MaterialPropertyName _gp_name;
  const ADMaterialProperty<Real> & _gp;
  const ADMaterialProperty<Real> & _dgp_dd;
  // @}
};
