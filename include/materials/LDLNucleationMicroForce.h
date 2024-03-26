//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"
#include "DerivativeMaterialPropertyNameInterface.h"

/**
 * The class implements the external driving force to recover a Drucker-Prager
 * strength envelope. See Larsen et. al. https://doi.org/10.48550/arXiv.2401.13938 for model 2024.
 * all parameters are required to be Material type, not double type
 */

class LDLNucleationMicroForce : public Material,
                                public BaseNameInterface,
                                public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  LDLNucleationMicroForce(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// Name of the external driving force
  const MaterialPropertyName _ex_driving_name;

  /// The external driving force
  ADMaterialProperty<Real> & _ex_driving;

  ///@{ Phase field properties
  /// The fracture toughness
  const ADMaterialProperty<Real> & _Gc;
  /// phase field variable
  const VariableValue & _d;
  /// degradation function name
  const MaterialPropertyName _g_name;
  /// degradation function
  const ADMaterialProperty<Real> & _g;
  /// The normalization constant
  const ADMaterialProperty<Real> & _c0;
  /// phase field regularization length
  const ADMaterialProperty<Real> & _L;
  ///@}

  /// Lame's first parameter
  const ADMaterialProperty<Real> & _lambda;
  /// The shear modulus
  const ADMaterialProperty<Real> & _mu;

  /// The critical tensile strength
  const ADMaterialProperty<Real> & _sigma_ts;

  /// The critical hydrostatic strength
  const ADMaterialProperty<Real> & _sigma_hs;

  /// The materiel and model dependent parameter
  ADMaterialProperty<Real> & _delta;

  /// Whether to use h correction formula for delta
  bool _h_correction;

  /// The stress tensor
  const ADMaterialProperty<RankTwoTensor> & _stress;

  /// Name of the stress space balance
  const MaterialPropertyName _stress_balance_name;
  /// Quantifying how far is the stress state from stress surface
  ADMaterialProperty<Real> & _stress_balance;
};