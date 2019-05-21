//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADMaterial.h"
#include "DerivativeMaterialPropertyNameInterface.h"

#define usingDegradationBaseMembers                                                                \
  usingMaterialMembers;                                                                            \
  using DegradationBase<compute_stage>::_d;                                                        \
  using DegradationBase<compute_stage>::_g_name;                                                   \
  using DegradationBase<compute_stage>::_g;                                                        \
  using DegradationBase<compute_stage>::_dg_dd;                                                    \
  using DegradationBase<compute_stage>::_eta;

// Forward Declarations
template <ComputeStage>
class DegradationBase;

declareADValidParams(DegradationBase);

template <ComputeStage compute_stage>
class DegradationBase : public ADMaterial<compute_stage>,
                        public DerivativeMaterialPropertyNameInterface
{
public:
  DegradationBase(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;
  virtual void computeDegradation(ADReal &) = 0;
  virtual void postComputeDegradation();

  /// coupled damage variable
  const ADVariableValue & _d;

  /// name of degradation
  const MaterialPropertyName _g_name;

  /// degradation
  ADMaterialProperty(Real) & _g;

  /// degradation derivative
  ADMaterialProperty(Real) & _dg_dd;

  /// residual degradation
  const Real & _eta;

  usingMaterialMembers;
};
