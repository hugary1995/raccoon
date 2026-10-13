//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SolutionChangeNormFixedPoint.h"
#include "Moose.h"
#include "MooseVariableFE.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

registerMooseObject("raccoonApp", SolutionChangeNormFixedPoint);

InputParameters
SolutionChangeNormFixedPoint::validParams()
{
  InputParameters params = NodalVariablePostprocessor::validParams();
  params.addClassDescription(
      "Computes the maximum nodal change in a variable between consecutive fixed-point "
      "(Picard) iterations: max_i |d_i^(k) - d_i^(k-1)|. Intended for use as a staggered-scheme "
      "convergence criterion via PostprocessorConvergence.");
  params.addParam<bool>(
      "print_history",
      true,
      "Echo the full fixed-point iteration history for the current time step on every iteration, "
      "so the convergence pattern can be followed in the terminal.");
  // Default to running at the fixed-point convergence check, after each sub-app solve and
  // damage transfer, but before the convergence decision is made.
  params.set<ExecFlagEnum>("execute_on") = {EXEC_MULTIAPP_FIXED_POINT_CONVERGENCE};
  return params;
}

SolutionChangeNormFixedPoint::SolutionChangeNormFixedPoint(const InputParameters & parameters)
  : NodalVariablePostprocessor(parameters),
    _fp_var(mooseVariable()),
    _sys_num(_fp_var->sys().number()),
    _var_num(_fp_var->number()),
    _max_change(0.0),
    _print_history(getParam<bool>("print_history"))
{
  // Ask the problem to save the previous fixed-point iteration solution for the system owning
  // 'variable'. This MUST happen in the constructor: FixedPointSolve::initialSetup() builds its
  // list of systems to copy from these flags, so requesting later is silently ignored and the
  // previous-iteration vector would stay zero for the whole run.
  auto & sys = _fp_var->sys();
  if (sys.varKind() == Moose::VarKindType::VAR_AUXILIARY)
    _fe_problem.needsPreviousMultiAppFixedPointIterationAuxiliary(true);
  else
    _fe_problem.needsPreviousMultiAppFixedPointIterationSolution(true, _sys_num);

  sys.needSolutionState(1, Moose::SolutionIterationType::MultiAppFixedPoint);
}

void
SolutionChangeNormFixedPoint::initialSetup()
{
  NodalVariablePostprocessor::initialSetup();

  // Guard against the silent failure mode: if the previous fixed-point solution is not being
  // copied, solutionState(1, FixedPoint) stays zero and this postprocessor would quietly report
  // max|d^(k)| instead of max|d^(k) - d^(k-1)|, which never converges.
  const auto & sys = _fp_var->sys();
  const bool is_copied =
      (sys.varKind() == Moose::VarKindType::VAR_AUXILIARY)
          ? _fe_problem.needsPreviousMultiAppFixedPointIterationAuxiliary()
          : _fe_problem.needsPreviousMultiAppFixedPointIterationSolution(_sys_num);
  if (!is_copied)
    mooseError("The previous fixed-point iteration solution for variable '",
               _fp_var->name(),
               "' is not being saved, so the fixed-point change cannot be computed.");
}

void
SolutionChangeNormFixedPoint::timestepSetup()
{
  // Start a fresh history for each time step. A step that fails and is retried with a smaller
  // dt re-enters here, so the retry gets its own history rather than appending to the failure.
  _history.clear();
}

void
SolutionChangeNormFixedPoint::initialize()
{
  _max_change = 0.0;

  const auto & sys = _fp_var->sys();

  // solutionState(0) is the current solution, updated by the MultiAppCopyTransfer to d^(k).
  sys.solutionState(0).localize(_current_local);

  // solutionState(1, FixedPoint) is the PREVIOUS_FP_SOLUTION_TAG vector — d^(k-1), copied
  // there by SystemBase::copyPreviousFixedPointSolutions() before the current solve began.
  sys.solutionState(1, Moose::SolutionIterationType::MultiAppFixedPoint).localize(_fp_old_local);
}

void
SolutionChangeNormFixedPoint::execute()
{
  // Skip nodes where the variable is not defined (e.g. block-restricted variables).
  if (_current_node->n_dofs(_sys_num, _var_num) == 0)
    return;

  const auto dof = _current_node->dof_number(_sys_num, _var_num, 0);
  _max_change = std::max(_max_change, std::abs(_current_local[dof] - _fp_old_local[dof]));
}

void
SolutionChangeNormFixedPoint::threadJoin(const UserObject & y)
{
  const auto & pps = static_cast<const SolutionChangeNormFixedPoint &>(y);
  _max_change = std::max(_max_change, pps._max_change);
}

Real
SolutionChangeNormFixedPoint::getValue() const
{
  return _max_change;
}

void
SolutionChangeNormFixedPoint::finalize()
{
  gatherMax(_max_change);

  // gatherMax() has synchronized the value, so every rank appends the same history.
  _history.push_back(_max_change);

  if (!_print_history)
  {
    _console << "  " << name() << " = " << _max_change << std::endl;
    return;
  }

  std::ostringstream oss;
  oss << "  " << name() << " history (time = " << _fe_problem.time() << "):\n";
  for (std::size_t i = 0; i < _history.size(); ++i)
  {
    oss << "    iter " << std::setw(3) << i << " : " << std::scientific << std::setprecision(6)
        << _history[i];
    // Ratio to the previous iteration: < 1 means the staggered scheme is contracting.
    if (i > 0 && _history[i - 1] > 0.0)
      oss << "   ratio " << std::fixed << std::setprecision(3) << (_history[i] / _history[i - 1]);
    oss << "\n";
  }
  _console << oss.str() << std::flush;
}
