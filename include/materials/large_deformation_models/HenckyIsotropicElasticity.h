//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "LargeDeformationElasticityModel.h"
#include "DerivativeMaterialPropertyNameInterface.h"

class HenckyIsotropicElasticity : public LargeDeformationElasticityModel,
                                  public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  HenckyIsotropicElasticity(const InputParameters & parameters);

  virtual ADRankTwoTensor computeMandelStress(const ADRankTwoTensor & Fe,
                                              const bool plasticity_update = false) override;

protected:
  // @{ Decomposition methods
  virtual ADRankTwoTensor computeMandelStressNoDecomposition(const ADRankTwoTensor & Fe,
                                                             const bool plasticity_update);
  virtual ADRankTwoTensor computeMandelStressVolDevDecomposition(const ADRankTwoTensor & Fe,
                                                                 const bool plasticity_update);
  // @}

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

  /// Decomposittion types
  const enum class Decomposition { none, spectral, voldev } _decomposition;
};
