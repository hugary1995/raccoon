//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElementIntegralPostprocessor.h"
#include "MooseVariableInterface.h"

// Forward Declarations
class ThermalAblationVolume;

template <>
InputParameters validParams<ThermalAblationVolume>();

/**
 * This postprocessor computes the thermal ablation volume.
 */
class ThermalAblationVolume : public ElementIntegralPostprocessor,
                              public MooseVariableInterface<Real>
{
public:
  ThermalAblationVolume(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral() override;

  /// damage
  const VariableValue & _d;
  /// temperature
  const VariableValue & _T;
  /// temperature threshold upon which the volume is considered to be removed by thermal ablation
  const Real _threshold;
};
