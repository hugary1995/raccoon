//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "FPSolve.h"

#include "NonlinearSystemBase.h"

InputParameters
FPSolve::validParams()
{
  InputParameters params = emptyInputParameters();

  params.addParam<unsigned int>("fp_max_its", 50, "Max Linear Iterations");
  params.addParam<Real>("fp_abs_tol", 1.0e-50, "Nonlinear Absolute Tolerance");
  params.addParam<Real>("fp_rel_tol", 1.0e-8, "Nonlinear Relative Tolerance");
  params.addParam<Real>("fp_abs_step_tol", 1.0e-50, "Nonlinear Absolute step Tolerance");
  params.addParam<Real>("fp_rel_step_tol", 1.0e-50, "Nonlinear Relative step Tolerance");
  params.addParamNamesToGroup("fp_max_its fp_abs_tol fp_rel_tol fp_abs_step_tol fp_rel_step_tol",
                              "FPSolve");
  return params;
}

FPSolve::FPSolve(Executioner * ex)
  : SolveObject(ex),
    _fp_problem(dynamic_cast<FixedPointProblem &>(_problem)),
    _fp_max_its(getParam<unsigned int>("fp_max_its")),
    _fp_abs_tol(getParam<Real>("fp_abs_tol")),
    _fp_rel_tol(getParam<Real>("fp_rel_tol")),
    _fp_abs_step_tol(getParam<Real>("fp_abs_step_tol")),
    _fp_rel_step_tol(getParam<Real>("fp_rel_step_tol"))
{
}

bool
FPSolve::solve()
{
  // evaluate kernels on previous solution
  _fp_problem.computeFullResidual(*_nl.currentSolution(), _nl.RHS());
  Real initial_residual_norm = _nl.RHS().l2_norm();
  Real residual_norm = initial_residual_norm;
  _console << "Fixed point initial residual norm " << residual_norm << std::endl;

  unsigned int it = 0;
  for (; it < _fp_max_its; ++it)
  {
    _console << "Fixed point iteration " << it << std::endl;

    Real residual_norm_previous = residual_norm;
    if (!_inner_solve->solve())
    {
      _console << COLOR_RED << " Fixed point iteration did NOT converge!" << COLOR_DEFAULT
               << std::endl;
      // Perform the output of the current, failed time step (this only occurs if desired)
      _fp_problem.outputStep(EXEC_FAILED);
      return false;
    }

    _fp_problem.computeFullResidual(*_nl.currentSolution(), _nl.RHS());
    residual_norm = _nl.RHS().l2_norm();
    _console << "Fixed point residual norm " << residual_norm << std::endl;
    if (residual_norm < _fp_abs_tol)
    {
      _console << COLOR_GREEN << " Fixed point iteration converged with fp_abs_tol!"
               << COLOR_DEFAULT << std::endl;
      break;
    }
    if (residual_norm / initial_residual_norm < _fp_rel_tol)
    {
      _console << COLOR_GREEN << " Fixed point iteration converged with fp_rel_tol!"
               << COLOR_DEFAULT << std::endl;
      break;
    }
    if (std::abs(residual_norm - residual_norm_previous) < _fp_abs_step_tol)
    {
      _console << COLOR_GREEN << " Fixed point iteration converged with fp_abs_step_tol!"
               << COLOR_DEFAULT << std::endl;
      break;
    }
    if (std::abs(residual_norm - residual_norm_previous) / residual_norm < _fp_rel_step_tol)
    {
      _console << COLOR_GREEN << " Fixed point iteration converged with fp_rel_step_tol!"
               << COLOR_DEFAULT << std::endl;
      break;
    }
  }

  return true;
}
