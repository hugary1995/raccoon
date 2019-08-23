//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "TransientSupNorm.h"

// MOOSE includes
#include "NonlinearSystem.h"

registerMooseObject("raccoonApp", TransientSupNorm);

template <>
InputParameters
validParams<TransientSupNorm>()
{
  InputParameters params = validParams<Transient>();
  return params;
}

TransientSupNorm::TransientSupNorm(const InputParameters & parameters) : Transient(parameters) {}

Real
TransientSupNorm::relativeSolutionDifferenceNorm()
{
  const NumericVector<Number> & current_solution = *_nl.currentSolution();
  const NumericVector<Number> & old_solution = _nl.solutionOld();

  _sln_diff = current_solution;
  _sln_diff -= old_solution;

  // Real norm = _sln_diff.linfty_norm() / old_solution.linfty_norm();
  Real norm = _sln_diff.l2_norm() / old_solution.l2_norm();

  if (std::isnan(norm))
    return 0.0;

  return norm * _dt;
}
