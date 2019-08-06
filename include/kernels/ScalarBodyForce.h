//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "Kernel.h"

// Forward Declarations
class ScalarBodyForce;

template <>
InputParameters validParams<ScalarBodyForce>();

/**
 * This kernel implements a generic functional
 * body force term:
 * $ - c \cdof f \cdot \phi_i $
 *
 * The coefficient and function both have defaults
 * equal to 1.0.
 */
class ScalarBodyForce : public Kernel
{
public:
  ScalarBodyForce(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;

  /// Scale factor
  const VariableValue & _scalar;
};
