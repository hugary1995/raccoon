//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "FPIterations.h"
#include "MooseApp.h"
#include "SubProblem.h"

registerMooseObject("raccoonApp", FPIterations);

defineLegacyParams(FPIterations);

InputParameters
FPIterations::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();
  params.addParam<bool>(
      "accumulate_over_step",
      false,
      "When set to true, accumulates to count the total over all Picard iterations for each step");
  params.addClassDescription("Outputs the number of nonlinear iterations");
  return params;
}

FPIterations::FPIterations(const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
    _fp_problem(dynamic_cast<FixedPointProblem *>(&_subproblem)),
    _ex(NULL),
    _accumulate_over_step(getParam<bool>("accumulate_over_step")),
    _num_iters(0),
    _time(-std::numeric_limits<Real>::max())
{
  if (!_fp_problem)
    mooseError("FPIterations postprocessor can only run on FixedPointProblem");
}

void
FPIterations::initialize()
{
  _ex = dynamic_cast<FixedPointTransient *>(_app.getExecutioner());
  if (!_ex)
  {
    mooseError(
        "The FPIterations Postprocessor can only be used with a FixedPointTransient Executioner");
  }
}

void
FPIterations::timestepSetup()
{
  if (_fp_problem->time() != _time)
  {
    _num_iters = 0;
    _time = _fp_problem->time();
  }
}

Real
FPIterations::getValue()
{
  if (_accumulate_over_step)
    _num_iters += _ex->getSolveObj().numFixedPointIts();
  else
    _num_iters = _ex->getSolveObj().numFixedPointIts();

  return _num_iters;
}
