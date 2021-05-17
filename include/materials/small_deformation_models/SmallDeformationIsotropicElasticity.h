//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "SmallDeformationElasticityModel.h"
#include "DerivativeMaterialPropertyNameInterface.h"

class SmallDeformationIsotropicElasticity : public SmallDeformationElasticityModel,
                                            public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  SmallDeformationIsotropicElasticity(const InputParameters & parameters);

  virtual ADRankTwoTensor computeStress(const ADRankTwoTensor & strain) override;

protected:
private:
  // @{ Decomposition methods
  virtual ADRankTwoTensor computeStressNoDecomposition(const ADRankTwoTensor & strain);
  virtual ADRankTwoTensor computeStressSpectralDecomposition(const ADRankTwoTensor & strain);
  virtual ADRankTwoTensor computeStressVolDevDecomposition(const ADRankTwoTensor & strain);
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
