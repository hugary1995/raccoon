//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADMaterial.h"
#include "DerivativeMaterialPropertyNameInterface.h"

#define usingDegradationBaseMembers                                                                \
  usingMaterialMembers;                                                                            \
  using DegradationBase<compute_stage>::_d;                                                        \
  using DegradationBase<compute_stage>::_d_old;                                                    \
  using DegradationBase<compute_stage>::_g_name;                                                   \
  using DegradationBase<compute_stage>::_g;                                                        \
  using DegradationBase<compute_stage>::_dg_dd;                                                    \
  using DegradationBase<compute_stage>::_eta;                                                      \
  using DegradationBase<compute_stage>::_lag;

// Forward Declarations
template <ComputeStage>
class DegradationBase;

declareADValidParams(DegradationBase);

template <ComputeStage compute_stage>
class DegradationBase : public ADMaterial<compute_stage>,
                        public DerivativeMaterialPropertyNameInterface
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
  ADMaterialProperty(Real) & _g;

  /// degradation derivative
  ADMaterialProperty(Real) & _dg_dd;

  /// residual degradation
  const Real & _eta;

  /// whether to lag the degradation
  const bool _lag;

  usingMaterialMembers;
};
