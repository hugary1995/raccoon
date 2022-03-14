//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "NewtonRaphsonSolve.h"

#include "Moose.h"
#include "MooseEnum.h"
#include "MooseObject.h"
#include "ConsoleStreamInterface.h"
#include "MathUtils.h"

InputParameters
NewtonRaphsonSolve::validParams()
{
  InputParameters params = emptyInputParameters();
  params.addParam<Real>(
      "nr_rel_tol", 1e-8, "Relative convergence tolerance for Newton-Raphson iteration");
  params.addParam<Real>(
      "nr_abs_tol", 1e-11, "Absolute convergence tolerance for Newton-Raphson iteration");
  params.addParam<unsigned int>("nr_max_its", 1000, "Maximum number of Newton-Raphson iterations");

  MooseEnum nr_output_on("NEVER FAILURE ALWAYS", "FAILURE");
  params.addParam<MooseEnum>(
      "nr_output_on", nr_output_on, "When to output Newton-Raphson solve information");
  params.addParamNamesToGroup("nr_output_on", "Debug");

  return params;
}

NewtonRaphsonSolve::NewtonRaphsonSolve(const InputParameters & parameters)
  : _nr_output_on(parameters.get<MooseEnum>("nr_output_on").getEnum<OutputOn>()),
    _rtol(parameters.get<Real>("nr_rel_tol")),
    _atol(parameters.get<Real>("nr_abs_tol")),
    _max_its(parameters.get<Real>("nr_max_its")),
    _svrms_name(parameters.get<std::string>("_object_name"))
{
}

void
NewtonRaphsonSolve::NRSolve(ADReal & u, const ConsoleStream & console)
{
  // Construct the stringstream here only if the debug level is set to ALWAYS
  std::unique_ptr<std::stringstream> iter_output =
      (_nr_output_on == OutputOn::ALWAYS) ? std::make_unique<std::stringstream>() : nullptr;

  // Do the internal solve and capture iteration info during the first round
  // iff full history output is requested regardless of whether the solve failed or succeeded
  auto solve_state = internalSolve(u, iter_output);

  // Output iteration information as requested
  switch (_internal_solve_output_on)
  {
    case InternalSolveOutput::NEVER:
      mooseException("");
      break;

    case InternalSolveOutput::FAILURE:
      if (solve_state != SolveState::SUCCESS)
      {
        iter_output = std::make_unique<std::stringstream>();
        internalSolve(u, iter_output);
      }

    case InternalSolveOutput::ALWAYS:
      mooseException(iter_output->str());
      break;

    default:
      mooseError("Internal error. Unhandled nr_output_on");
  }
}

typename NewtonRaphsonSolve::SolveState
NewtonRaphsonSolve::internalSolve(ADReal & u, std::stringstream * iter_output)
{
  const Real lb = NRLowerBound();
  const Real ub = NRUpperBound();

  if (u < lb)
    u = lb;
  if (u > ub)
    u = ub;

  unsigned int it = 0;
  ADReal p = 0;

  ADReal rJ = NRResidual(u);
  ADReal r0 = r = rJ.value();
  ADReal J = rJ.derivative();

  outputIteration(iter_output, it, u, r);

  if (converged(r, r0))
    return SolveState::SUCCESS;
  else if (std::isnan(r) || std::isinf(r))
    return SolveState::NAN_INF;

  while (it < _max_its)
  {
    ++it;
    p = -r / J;
    Real alpha = lineSearch(u, p, r, lb, ub, iter_output);
    u += alpha * p;

    ADReal rJ = NRResidual(u);
    ADReal r = rJ.value();
    ADReal J = rJ.derivative();

    outputIteration(iter_output, it, u, r);

    if (converged(r, r0))
      return SolveState::SUCCESS;
    else if (std::isnan(r) || std::isinf(r))
      return SolveState::NAN_INF;
  }

  return SolveState::EXCEEDED_ITERATIONS;
}

bool
NewtonRaphsonSolve::converged(const ADReal & r, const ADReal & r0)
{
  if (std::abs(raw_value(r)) <= _atol)
    return true;

  if (std::abs(raw_value(r)) <= _rtol * std::abs(raw_value(r0)))
    return true;

  return false;
}

Real
NewtonRaphsonSolve::lineSearch(const ADReal & u,
                               const ADReal & p,
                               const ADReal & /*r*/,
                               const Real lb,
                               const Real ub,
                               std::stringstream * iter_output)
{
  // Start with a full NR step
  Real alpha = 1;

  // There are three conditions for the line search:
  // 1. The Armijo condition insisting that the residual has dropped sufficiently
  // 2. The curvature condition insisting that the step makes reasonable progress
  // 3. The step stays within the bounds
  // Here, we relax the first two conditions and we only do a simple bound checking.
  while (u + alpha * p > ub || u + alpha * p < lb)
  {
    alpha *= 0.5;
    if (iter_output)
      *iter_output << "Line search alpha = " alpha << std::endl;
  }

  return alpha;
}

void
NewtonRaphsonSolve::outputIteration(std::stringstream * iter_output,
                                    const unsigned int it,
                                    const ADReal & u,
                                    const ADReal & r)
{
  if (iter_output)
    *iter_output << " " << it << ", u =" << raw_value(u) << ", |R| = " << std::abs(raw_value(r))
                 << std::endl;
}
