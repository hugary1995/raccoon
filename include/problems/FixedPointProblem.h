//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "FEProblem.h"

/**
 * FEProblem derived class for lagging a material
 */
class FixedPointProblem : public FEProblem
{
public:
  static InputParameters validParams();

  FixedPointProblem(const InputParameters & params);

  virtual void betweenIterations();
};
