//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "DerivativeMaterialPropertyNameInterface.h"

template <bool is_ad>
class DegradationBaseTempl : public Material, public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  DegradationBaseTempl(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;
  virtual void computeDegradation() = 0;
  virtual void postComputeDegradation();

  /// coupled damage variable
  const GenericVariableValue<is_ad> & _d;

  /// last converged damage variable
  const VariableValue & _d_old;

  /// name of degradation
  const MaterialPropertyName _g_name;

  /// degradation
  GenericMaterialProperty<Real, is_ad> & _g;

  /// degradation derivative
  GenericMaterialProperty<Real, is_ad> & _dg_dd;

  /// residual degradation
  const Real & _eta;

  /// whether to lag the degradation
  const bool _lag;
};

typedef DegradationBaseTempl<false> DegradationBase;
typedef DegradationBaseTempl<true> ADDegradationBase;
