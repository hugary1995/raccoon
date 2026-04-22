//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "LargeDeformationPlasticityModel.h"
#include "DerivativeMaterialPropertyNameInterface.h"

class LargeDeformationJ2PlasticityBeBar : public LargeDeformationPlasticityModel,
                                          public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  LargeDeformationJ2PlasticityBeBar(const InputParameters & parameters);

  virtual void updateState(ADRankTwoTensor & stress, ADRankTwoTensor & Fe) override;

  virtual void setElasticityModel(LargeDeformationElasticityModel * elasticity_model) override;

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

  ADMaterialProperty<RankTwoTensor> & _bebar;
  const MaterialProperty<RankTwoTensor> & _bebar_old;
  ADMaterialProperty<Real> & _bebar_det;

  /// Pointers to elastic properties sourced from the CNH elasticity model via setElasticityModel.
  const ADMaterialProperty<Real> * _K = nullptr;
  const ADMaterialProperty<Real> * _G = nullptr;
  const ADMaterialProperty<Real> * _ge = nullptr;
  const ADMaterialProperty<Real> * _dge_dd = nullptr;

  /// Writable references to CNH's declared energy properties, overwritten with bebar-based values.
  ADMaterialProperty<Real> * _psie_cnh = nullptr;
  ADMaterialProperty<Real> * _psie_active_cnh = nullptr;
  ADMaterialProperty<Real> * _dpsie_dd_cnh = nullptr;

  const MaterialProperty<RankTwoTensor> & _F_old;
  const ADMaterialProperty<RankTwoTensor> & _F;

  const VariableName _d_name;

  const MaterialPropertyName _psie_name;
  ADMaterialProperty<Real> & _psie_corr;
  ADMaterialProperty<Real> & _psie_active_corr;
  ADMaterialProperty<Real> & _dpsie_dd_corr;
  ADMaterialProperty<Real> & _psie_unsplit;

  const bool _apply_strain_energy_split;
};
