//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "TransientSubcycling.h"
#include "NonlinearSystem.h"

registerMooseObject("raccoonApp", TransientSubcycling);

InputParameters
TransientSubcycling::validParams()
{
  InputParameters params = Transient::validParams();
  return params;
}

TransientSubcycling::TransientSubcycling(const InputParameters & parameters) : Transient(parameters)
{
}

Real
TransientSubcycling::relativeSolutionDifferenceNorm()
{
  const NumericVector<Number> & current_solution = *_nl.currentSolution();
  const NumericVector<Number> & old_solution = _nl.solutionOld();

  _sln_diff = current_solution;
  _sln_diff -= old_solution;

  Real norm = _sln_diff.linfty_norm() / old_solution.linfty_norm();
  // Real norm = _sln_diff.l2_norm() / old_solution.l2_norm();

  if (std::isnan(norm))
    return 0.0;

  _console << COLOR_BLUE << "\n===============================================\n" << COLOR_DEFAULT;
  _console << " relative solution change norm: " << std::scientific << COLOR_BLUE << norm
           << COLOR_DEFAULT << std::endl;
  _console << "solution change norm tolerance: " << std::scientific << COLOR_BLUE
           << _steady_state_tolerance << COLOR_DEFAULT << std::endl;
  _console << COLOR_BLUE << "===============================================\n"
           << COLOR_DEFAULT << std::flush;
  fflush(stdout);

  return norm * _dt;
}
