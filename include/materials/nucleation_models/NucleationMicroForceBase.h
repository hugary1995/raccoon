//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"
#include "DerivativeMaterialPropertyNameInterface.h"

class NucleationMicroForceBase : public Material,
                                 public BaseNameInterface,
                                 public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();
  NucleationMicroForceBase(const InputParameters & parameters);

protected:
  ///@{ fracture properties
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

  /// The stress tensor
  const ADMaterialProperty<RankTwoTensor> & _stress;
  /// Name of the stress space balance
  const MaterialPropertyName _stress_balance_name;
  /// Quantifying how far is the stress state from stress surface
  ADMaterialProperty<Real> & _stress_balance;

  /// Name of the external driving force
  const MaterialPropertyName _ex_driving_name;
  /// The external nucleation driving force
  ADMaterialProperty<Real> & _ex_driving;

  /// @{ The degradation function and its derivative w/r/t damage
  const VariableName _d_name;
  const MaterialPropertyName _g_name;
  const ADMaterialProperty<Real> & _g;
  const ADMaterialProperty<Real> & _dg_dd;
  /// @}
};
