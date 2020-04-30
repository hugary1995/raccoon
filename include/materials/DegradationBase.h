//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADMaterial.h"
#include "DerivativeMaterialPropertyNameInterface.h"

class DegradationBase : public ADMaterial, public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  DegradationBase(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;
  virtual void computeDegradation() = 0;
  virtual void postComputeDegradation();

  /// coupled damage variable
  const ADVariableValue & _d;

  /// last converged damage variable
  const VariableValue & _d_old;

  /// name of degradation
  const MaterialPropertyName _g_name;

  /// degradation
  ADMaterialProperty<Real> & _g;

  /// degradation derivative
  ADMaterialProperty<Real> & _dg_dd;

  /// residual degradation
  const Real & _eta;

  /// whether to lag the degradation
  const bool _lag;
};
