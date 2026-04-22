//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElementIntegralPostprocessor.h"
#include "MooseVariableInterface.h"

/**
 * Computes the L2 norm of the change in a variable between consecutive fixed-point
 * (Picard) iterations: ||d^(k+1) - d^(k)||_2.
 *
 * Intended for use as a staggered-scheme convergence criterion via PostprocessorConvergence,
 * referenced via the Executioner parameter `multiapp_fixed_point_convergence`.
 *
 * Should execute at MULTIAPP_FIXED_POINT_CONVERGENCE (the default) so it evaluates
 * after the sub-app has solved and the damage field has been transferred back, but
 * before the convergence decision is made.
 */
class SolutionChangeNormFixedPoint : public ElementIntegralPostprocessor,
                                     public MooseVariableInterface<Real>
{
public:
  static InputParameters validParams();

  SolutionChangeNormFixedPoint(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void finalize() override;
  virtual Real getValue() const override;

protected:
  virtual Real computeQpIntegral() override;

  // The FE variable being tracked across fixed-point iterations
  MooseVariableFE<Real> * _fp_var;

  // Localized copy of the current solution (post-transfer), indexed by global DOF.
  // Must use solution() directly — coupledValue() reads current_local_solution which is
  // not synced after a MultiAppCopyTransfer writes to solution().
  std::vector<Number> _current_local;

  // Localized copy of the previous fixed-point iteration solution, indexed by global DOF.
  std::vector<Number> _fp_old_local;
};
