//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

// MOOSE Includes
#include "InitialCondition.h"

/**
 * PhasefieldShapes just returns a constant value.
 */
class PhasefieldShapes : public InitialCondition
{
public:
  static InputParameters validParams();
  PhasefieldShapes(const InputParameters & parameters);

  virtual Real value(const Point & p);

private:
  Real _epsilon;
  std::vector<Real> _centers_tips;
  std::vector<std::string> _shapes;
  std::vector<std::string> _gradients;
  std::vector<Real> _gradient_values;
  Real _fluid_phase;
};
