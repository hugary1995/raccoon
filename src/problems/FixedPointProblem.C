//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "FixedPointProblem.h"
#include "raccoonAppTypes.h"

registerMooseObject("raccoonApp", FixedPointProblem);

InputParameters
FixedPointProblem::validParams()
{
  InputParameters params = FEProblem::validParams();
  return params;
}

FixedPointProblem::FixedPointProblem(const InputParameters & params) : FEProblem(params) {}

void
FixedPointProblem::betweenIterations()
{
  execute(EXEC_BETWEEN_FPI);
}
