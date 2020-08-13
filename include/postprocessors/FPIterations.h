//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "GeneralPostprocessor.h"
#include "FixedPointProblem.h"
#include "FixedPointTransient.h"

// Forward Declarations
class FPIterations;

template <>
InputParameters validParams<FPIterations>();

/**
 * FPIterations is a postprocessor that reports the number of nonlinear iterations
 */

class FPIterations : public GeneralPostprocessor
{
public:
  static InputParameters validParams();

  FPIterations(const InputParameters & parameters);

  /**
   * Initialization to be done at each timestep
   */
  virtual void timestepSetup() override;

  virtual void initialize() override;
  virtual void execute() override {}

  /**
   * Get the numer of nonlinear iterations
   */
  virtual Real getValue() override;

protected:
  /// Pointer to the FEProblemBase
  FixedPointProblem * _fp_problem;
  FixedPointTransient * _ex;

  /// True if we should accumulate over all nonlinear solves done as part of Picard iterations in a step.
  bool _accumulate_over_step;

  /// Stores the nonlinear iteration count
  unsigned int _num_iters;

  /// Stores the last time this was executed
  Real _time;
};
