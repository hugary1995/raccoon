//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "SolveObject.h"
#include "FixedPointProblem.h"

// System includes
#include <string>

class FixedPointSolve : public SolveObject
{
public:
  static InputParameters validParams();

  FixedPointSolve(Executioner * ex);

  virtual bool solve() override;

  /// Enumeration for Fixed Point convergence reasons
  enum class MooseFPConvergenceReason
  {
    UNSOLVED = 0,
    CONVERGED = 1,
    REACH_MAX_ITS = 2,
    DIVERGED_MAX_ITS = -1
  };

  /**
   * Get the number of fixed point iterations performed
   * Because this returns the number of fixed point iterations, rather than the current
   * iteration count (which starts at 0), increment by 1.
   *
   * @return Number of fixed point iterations performed
   */
  unsigned int numFixedPointIts() const { return _fp_it; }

  /// Check the solver status
  MooseFPConvergenceReason checkConvergence() const { return _fp_status; }

  /// Clear fixed point iteration status
  void clearFixedPointStatus() { _fp_status = MooseFPConvergenceReason::UNSOLVED; }

protected:
  void initializeSolutionDifferenceNorm();
  Real computeSolutionDifferenceNorm();
  /// Underlying Fixed Point problem
  FixedPointProblem & _fp_problem;
  /// Maximum number of fixed point iterations
  unsigned int _fp_max_its;
  /// Whether or not to treat reaching maximum number of fixed point iteration as converged
  bool _accept_max_it;
  /// tolerance on residual norm
  Real _fp_tol;

private:
  /// Fixed point iteration counter
  unsigned int _fp_it;
  /// Full history of residual norm
  std::vector<Real> _fp_norm;
  /// Status of Fixed Point solve
  MooseFPConvergenceReason _fp_status;

  /// The difference of current and old solutions
  NumericVector<Number> & _sln_diff;

  const std::string _solve_message;
};
