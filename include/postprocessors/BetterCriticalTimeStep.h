//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElementPostprocessor.h"

template <bool is_ad>
class BetterCriticalTimeStepTempl : public ElementPostprocessor
{
public:
  static InputParameters validParams();

  BetterCriticalTimeStepTempl(const InputParameters & parameters);

  virtual void initialize() override {}
  virtual void execute() override;

  virtual void finalize() override;
  virtual PostprocessorValue getValue() override;
  virtual void threadJoin(const UserObject & y) override;

protected:
  /// Density of the material
  const GenericMaterialProperty<Real, is_ad> & _material_density;

  /// Effective stiffness of element
  const GenericMaterialProperty<Real, is_ad> & _effective_stiffness;

  /// User defined factor to be multiplied to the critical time step
  const Real & _factor;

  /// Critical time step for explicit solver
  Real _critical_time;
  /// Material wave speed
  Real _c;
};

typedef BetterCriticalTimeStepTempl<false> BetterCriticalTimeStep;
typedef BetterCriticalTimeStepTempl<true> ADBetterCriticalTimeStep;
