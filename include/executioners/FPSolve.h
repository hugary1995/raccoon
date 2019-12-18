//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "SolveObject.h"

#include "FixedPointProblem.h"

class FPSolve : public SolveObject
{
public:
  static InputParameters validParams();

  FPSolve(Executioner * ex);

  virtual bool solve() override;

protected:
  FixedPointProblem & _fp_problem;
  const unsigned int _fp_max_its;
  const Real _fp_abs_tol;
  const Real _fp_rel_tol;
  const Real _fp_abs_step_tol;
  const Real _fp_rel_step_tol;
};
