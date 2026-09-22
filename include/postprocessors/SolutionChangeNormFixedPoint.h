//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "NodalVariablePostprocessor.h"

/**
 * Computes the maximum nodal change in a variable between consecutive fixed-point
 * (Picard) iterations: max_i |d_i^(k) - d_i^(k-1)|.
 *
 * An L-infinity measure is used rather than an integral L2 norm so that the value is
 * independent of domain size and of how far the crack has advanced. For a phase field
 * bounded in [0,1] the value is directly interpretable: a tolerance of 1e-3 means no
 * node's damage moved by more than 0.001 over the iteration.
 *
 * Intended for use as a staggered-scheme convergence criterion via PostprocessorConvergence,
 * referenced via the Executioner parameter `multiapp_fixed_point_convergence`.
 *
 * Should execute at MULTIAPP_FIXED_POINT_CONVERGENCE (the default) so it evaluates
 * after the sub-app has solved and the damage field has been transferred back, but
 * before the convergence decision is made.
 */
class SolutionChangeNormFixedPoint : public NodalVariablePostprocessor
{
public:
  static InputParameters validParams();

  SolutionChangeNormFixedPoint(const InputParameters & parameters);

  virtual void initialSetup() override;
  virtual void timestepSetup() override;
  virtual void initialize() override;
  virtual void execute() override;
  virtual void threadJoin(const UserObject & y) override;
  virtual void finalize() override;
  virtual Real getValue() const override;

protected:
  // The FE variable being tracked across fixed-point iterations
  MooseVariableFE<Real> * _fp_var;

  // System and variable numbers used to look up nodal DOFs
  unsigned int _sys_num;
  unsigned int _var_num;

  // Running maximum of |d^(k) - d^(k-1)| over nodes
  Real _max_change;

  // Whether to echo the full fixed-point iteration history each iteration
  const bool _print_history;

  // Values from every fixed-point iteration of the current time step, cleared in timestepSetup()
  std::vector<Real> _history;

  // Localized copy of the current solution (post-transfer), indexed by global DOF.
  // Must use solution() directly — coupledValue() reads current_local_solution which is
  // not synced after a MultiAppCopyTransfer writes to solution().
  std::vector<Number> _current_local;

  // Localized copy of the previous fixed-point iteration solution, indexed by global DOF.
  std::vector<Number> _fp_old_local;
};
