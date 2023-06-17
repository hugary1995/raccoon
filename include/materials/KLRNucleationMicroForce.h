//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"
#include "DerivativeMaterialPropertyNameInterface.h"

/**
 * The class implements the external driving force to recover a Drucker-Prager
 * strength envelope developed by Kumar et al. in 2022. See Kumar, A., Ravi-Chandar, K. &
 * Lopez-Pamies, O. The revisited phase-field approach to brittle fracture: application to
 * indentation and notch problems. Int J Fract 237, 83–100 (2022).
 * https://doi.org/10.1007/s10704-022-00653-z. all parameters are required to be Material type, not
 * double type
 */
class KLRNucleationMicroForce : public Material,
                                public BaseNameInterface,
                                public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  KLRNucleationMicroForce(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// Name of the external driving force
  const MaterialPropertyName _ex_driving_name;

  /// The external driving force
  ADMaterialProperty<Real> & _ex_driving;

  ///@{ Phase field properties
  /// The fracture toughness
  const ADMaterialProperty<Real> & _Gc;
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

  /// The critical compressive strength
  const ADMaterialProperty<Real> & _sigma_cs;

  /// The materiel and model dependent parameter
  const ADMaterialProperty<Real> & _delta;

  /// The stress tensor
  const ADMaterialProperty<RankTwoTensor> & _stress;

  /// Name of the stress space balance
  const MaterialPropertyName _stress_balance_name;
  /// Quantifying how far is the stress state from stress surface
  ADMaterialProperty<Real> & _stress_balance;

  /// Name of the phase-field variable
  const VariableName _d_name;
  // @{ The degradation function and its derivative w/r/t damage
  const MaterialPropertyName _g_name;
  const ADMaterialProperty<Real> & _g;
  const ADMaterialProperty<Real> & _dg_dd;
  // @}
};
