//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "MooseTypes.h"
#include "DualRealOps.h"
#include "InputParameters.h"

class ConsoleStream;

class NewtonRaphsonSolve
{
public:
  static InputParameters validParams();

  NewtonRaphsonSolve(const InputParameters & parameters);
  virtual ~NewtonRaphsonSolve() {}

protected:
  /**
   * Perform the Newton-Raphson iterations
   * @param u        The unknown being solved for
   * @param console  Console output
   */
  virtual void NRSolve(ADReal & u, const ConsoleStream & console);

  /// Set the bounds for the unknown variable
  // @{
  virtual Real NRLowerBound() const { return std::numeric_limits<Real>::lowest(); }
  virtual Real NRUpperBound() const { return std::numeric_limits<Real>::max(); }
  // @}

  /**
   * Compute the residual
   * @param u    The unknown variable being solved for
   */
  virtual ADReal NRResidual(const ADReal & u) = 0;

  /**
   * Output information for a single iteration step
   * @param iter_output            Output stream
   * @param it                     Current iteration number
   * @param u                      Current value of the unknown variable
   * @param r                      Current value of the residual
   */
  virtual void outputIteration(std::stringstream * iter_output,
                               const unsigned int it,
                               const ADReal & u,
                               const ADReal & r);

  /**
   * Check to see whether the residual is within the convergence limits.
   * @param r  Current value of the residual
   * @param r0 Value of the initial residual
   * @return Whether the model converged
   */
  bool converged(const ADReal & r, const ADReal & r0);

private:
  /**
   * Method called from within this class to perform the actual Newton-Raphson iterations.
   * @param u                      The unknown variable being solved for
   * @param iter_output            Output stream -- if null, no output is produced
   * @return The solution state, e.g. success, nan_inf, exceeded_iterations.
   */
  SolveState internalSolve(ADReal & u, std::stringstream * iter_output = nullptr);

  /**
   * Perform the line search
   * @param u           Current value of the unknown variable
   * @param p           Current step
   * @param r           Reference residual
   * @param lb          Lower bound
   * @param ub          Upper bound
   * @param iter_output Output stream
   * @return The step size multiplier after line search
   */
  void lineSearch(const ADReal & u,
                  const ADReal & p,
                  const ADReal & /*r*/,
                  const Real lb,
                  const Real ub,
                  std::stringstream * iter_output);

  const enum class OutputOn { NEVER, FAILURE, ALWAYS } _nr_output_on;

  const enum class SolveState { SUCCESS, NAN_INF, EXCEEDED_ITERATIONS };

  /// Relative convergence tolerance
  const Real _rtol;

  /// Absolute convergence tolerance
  const Real _atol;

  /// Maximum number of return mapping iterations. This exists only to avoid an infinite loop, and is
  /// is intended to be a large number that is not settable by the user.
  const unsigned int _max_its;

  /// MOOSE input name of the object performing the solve
  const std::string _svrms_name;
};
