//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "TimeStepper.h"
#include "FixedPointTransient.h"

registerMooseObject("raccoonApp", FixedPointTransient);

InputParameters
FixedPointTransient::validParams()
{
  InputParameters params = Transient::validParams();
  params += FPSolve::validParams();
  return params;
}

FixedPointTransient::FixedPointTransient(const InputParameters & parameters)
  : Transient(parameters), _fp_solve(this)
{
  _fp_solve.setInnerSolve(_picard_solve);
}

void
FixedPointTransient::init()
{
  Transient::init();
  _time_stepper->setInnerSolve(&_fp_solve);
}
