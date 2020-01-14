//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "FixedPointSolve.h"

#include "FEProblem.h"
#include "Executioner.h"
#include "NonlinearSystem.h"
#include "Console.h"

defineLegacyParams(FixedPointSolve);

InputParameters
FixedPointSolve::validParams()
{
  InputParameters params = emptyInputParameters();

  params.addParam<unsigned int>(
      "fp_max_its", 1, "Specifies the maximum number of fixed point iterations.");
  params.addParam<bool>(
      "accept_on_max_fp_iteration",
      true,
      "True to treat reaching the maximum number of fixed point iterations as converged.");
  params.addParam<Real>("fp_tol", 1e-06, "The tolerance for the solution difference L2 norm");

  return params;
}

FixedPointSolve::FixedPointSolve(Executioner * ex)
  : SolveObject(ex),
    _fp_problem(dynamic_cast<FixedPointProblem &>(_problem)),
    _fp_max_its(getParam<unsigned int>("fp_max_its")),
    _accept_max_it(getParam<bool>("accept_on_max_fp_iteration")),
    _fp_tol(getParam<Real>("fp_tol")),
    _fp_it(0),
    _fp_status(MooseFPConvergenceReason::UNSOLVED),
    _sln_diff(_nl.addVector("sln_diff", false, PARALLEL)),
    _solve_message(_fp_problem.shouldSolve() ? "Solve Converged!" : "Solve Skipped!")
{
}

bool
FixedPointSolve::solve()
{
  Real current_dt = _fp_problem.dt();

  _fp_norm.clear();
  _fp_norm.resize(_fp_max_its + 1);

  bool converged = true;

  // need to back up multi-apps even when not doing fixed point iteration for recovering from failed
  // multiapp solve
  _fp_problem.backupMultiApps(EXEC_TIMESTEP_BEGIN);
  _fp_problem.backupMultiApps(EXEC_TIMESTEP_END);

  for (_fp_it = 0; _fp_it < _fp_max_its + 1; ++_fp_it)
  {
    if (_fp_it != 0)
    {
      // For every iteration other than the first, we need to restore the state of the MultiApps
      _fp_problem.restoreMultiApps(EXEC_TIMESTEP_BEGIN);
      _fp_problem.restoreMultiApps(EXEC_TIMESTEP_END);
    }

    _console << COLOR_BLUE << "===================================\n";
    _console << "Fixed Point Iteration " << std::setw(std::ceil(std::log10(_fp_max_its)) + 1)
             << _fp_it << '\n';
    _console << "-----------------------------------\n" << COLOR_DEFAULT;

    bool solve_converged = _inner_solve->solve();

    if (solve_converged)
    {
      if (_fp_it == 0)
      {
        _fp_norm[0] = std::numeric_limits<Real>::max();
        initializeSolutionDifferenceNorm();
      }
      else
      {
        _fp_norm[_fp_it] = computeSolutionDifferenceNorm();
        _console << COLOR_BLUE << "-----------------------------------\n";
        _console << "Fixed Point |R| = " << std::scientific << _fp_norm[_fp_it] << '\n'
                 << COLOR_DEFAULT;
      }

      if (_fp_norm[_fp_it] < _fp_tol && _fp_it > 0)
      {
        _fp_status = MooseFPConvergenceReason::CONVERGED;
        break;
      }
      if (_fp_it == _fp_max_its)
      {
        if (_accept_max_it)
        {
          _fp_status = MooseFPConvergenceReason::REACH_MAX_ITS;
          converged = true;
        }
        else
        {
          _fp_status = MooseFPConvergenceReason::DIVERGED_MAX_ITS;
          converged = false;
        }
        break;
      }

      // fixed point solve hasn't converged yet
      _fp_problem.betweenIterations();
    }
    else
    {
      // If the last solve didn't converge then we need to exit this step completely
      converged = false;
      break;
    }

    _fp_problem.dt() =
        current_dt; // _dt might be smaller than this at this point for multistep methods
  }

  _console << COLOR_BLUE << "===================================\n" << COLOR_DEFAULT;
  _console << "Fixed Point converged reason: ";
  switch (_fp_status)
  {
    case MooseFPConvergenceReason::CONVERGED:
      _console << "CONVERGED";
      break;
    case MooseFPConvergenceReason::REACH_MAX_ITS:
      _console << "REACH_MAX_ITS";
      break;
    case MooseFPConvergenceReason::DIVERGED_MAX_ITS:
      _console << "DIVERGED_MAX_ITS";
      break;
    default:
      mooseError("Internal error: wrong Fixed Point status!");
      break;
  }
  _console << std::endl;
  return converged;
}

void
FixedPointSolve::initializeSolutionDifferenceNorm()
{
  const NumericVector<Number> & current_solution = *_nl.currentSolution();

  // current solutions becomes old solution next time
  _sln_diff = current_solution;
  _sln_diff *= -1.0;
}

Real
FixedPointSolve::computeSolutionDifferenceNorm()
{
  const NumericVector<Number> & current_solution = *_nl.currentSolution();
  _sln_diff += current_solution;
  Real diff_norm = _sln_diff.l2_norm();

  // current solutions becomes old solution next time
  _sln_diff = current_solution;
  _sln_diff *= -1.0;

  return diff_norm;
}
