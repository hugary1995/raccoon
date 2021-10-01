#pragma once

#include "BoundsAuxBase.h"

/**
 * Provides a conditional bound of a variable using its old value or a fixed
 * value.
 */
class ConditionalBoundsAux : public BoundsAuxBase
{
public:
  static InputParameters validParams();

  ConditionalBoundsAux(const InputParameters & parameters);

protected:
  virtual Real getBound() override;

  /// The value of the fixed bound for the variable
  Real _fixed_bound_value;

  /// The threshold for conditional bound for the variable
  Real _threshold_value;
};
