//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ElementPostprocessor.h"

/**
 * Compute the critical time step for an explicit integration scheme by inferring an
 * effective_stiffness from element classes and density from the user.
 */

// Forward Declarations

class BetterCriticalTimeStep : public ElementPostprocessor
{
public:
  static InputParameters validParams();

  BetterCriticalTimeStep(const InputParameters & parameters);

  virtual void initialize() override {}
  virtual void execute() override;

  virtual void finalize() override;
  virtual PostprocessorValue getValue() override;
  virtual void threadJoin(const UserObject & y) override;

protected:
  /// Density of the material
  const MaterialPropertyName _rho_name;
  const MaterialProperty<Real> & _material_density;

  /// Effective stiffness of element: function of material properties and element size
  const MaterialPropertyName _E_name;
  const MaterialProperty<Real> & _effective_stiffness;

  /// User defined factor to be multiplied to the critical time step
  const Real & _factor;

  /// Critical time step for explicit solver
  Real _critical_time;
  Real _c;
};
