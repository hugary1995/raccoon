//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "LargeDeformationPlasticityModel.h"
#include "DerivativeMaterialPropertyNameInterface.h"
#include "Qp_Mapping.h"

class LargeDeformationJ2PlasticityCorrection : public LargeDeformationPlasticityModel,
                                               public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  LargeDeformationJ2PlasticityCorrection(const InputParameters & parameters);

  virtual void updateState(ADRankTwoTensor & stress, ADRankTwoTensor & Fe) override;

protected:
  virtual ADReal initialGuess(const ADReal & effective_trial_stress) override
  {
    return _hardening_model->initialGuess(effective_trial_stress);
  }

  virtual void initQpStatefulProperties() override;

  virtual ADReal minimumPermissibleValue(const ADReal &) const override { return 0; }

  virtual ADReal computeResidual(const ADReal & effective_trial_stress,
                                 const ADReal & delta_ep) override;

  virtual ADReal computeDerivative(const ADReal & effective_trial_stress,
                                   const ADReal & delta_ep) override;

  virtual Real computeReferenceResidual(const ADReal & effective_trial_stress,
                                        const ADReal & delta_ep) override;

  void computeCorrectionTerm(const ADRankTwoTensor & devbebar);

  void computeStrainEnergyDensity();

  // Bebar state variable
  ADMaterialProperty<RankTwoTensor> & _bebar;
  const MaterialProperty<RankTwoTensor> & _bebar_old;
  ADMaterialProperty<Real> & _bebar_det;
  // ADMaterialProperty<RankTwoTensor> & _f;
  const ADMaterialProperty<Real> & _G;
  const ADMaterialProperty<Real> & _K;
  const MaterialProperty<RankTwoTensor> & _F_old;
  const ADMaterialProperty<RankTwoTensor> & _F;

  /// Name of the phase-field variable
  const VariableName _d_name;

  // @{ The degradation function and its derivative w/r/t damage
  const MaterialPropertyName _ge_name;
  const ADMaterialProperty<Real> & _ge;
  const ADMaterialProperty<Real> & _dge_dd;

  // @{ Strain energy density and its derivative w/r/t damage
  const MaterialPropertyName _psie_name;
  ADMaterialProperty<Real> & _psie_corr;
  ADMaterialProperty<Real> & _psie_active_corr;
  ADMaterialProperty<Real> & _dpsie_dd_corr;
  ADMaterialProperty<Real> & _psie_unsplit;
  // @}
  ADMaterialProperty<Real> & _triaxfunc;
  ADMaterialProperty<Real> & _triaxiality_kirchhoff;
  ADMaterialProperty<Real> & _triaxiality_cauchy;
  const Real _d1;
  const Real _d2;
  const Real _d3;
  const Real _triax_gaussian_peak;
  QpMapping::Element _element;
  const bool _apply_strain_energy_split;

  /// Active plastic energy density from the hardening model (psip_active)
  const ADMaterialProperty<Real> & _psip_active_ref;
  /// Activation threshold: psip_active/triaxfunc must exceed this before psip_triax is non-zero
  const Real _psip_triax_threshold;
  const bool _recover_psip_triax;
  /// Triaxiality-weighted plastic energy without threshold (stateful, used for recovery)
  ADMaterialProperty<Real> & _psip_triax_raw;
  const MaterialProperty<Real> & _psip_triax_raw_old;
  /// Triaxiality-weighted plastic energy with threshold applied (derived from _psip_triax_raw)
  ADMaterialProperty<Real> & _psip_triax;
  const MaterialProperty<Real> & _psip_triax_old;

  unsigned int _qpnum;

private:
  const std::unordered_map<int, int> * _lookup;
};
