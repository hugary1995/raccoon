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

  return _sln_diff.linfty_norm() * _dt;
}
