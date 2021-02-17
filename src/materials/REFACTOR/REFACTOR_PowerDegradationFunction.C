//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "REFACTOR_PowerDegradationFunction.h"

registerMooseObject("raccoonApp", REFACTOR_PowerDegradationFunction);

InputParameters
REFACTOR_PowerDegradationFunction::validParams()
{
  InputParameters params = REFACTOR_DegradationFunctionBase::validParams();
  params.addClassDescription(
      "defines the power degradation function $g(d) = (1-d)^p (1-\\eta) + \\eta$.");

  params.set<std::string>("function") = "(1-d)^p*(1-eta)+eta";

  const std::vector<std::string> default_params = {"p", "eta"};
  params.set<std::vector<std::string>>("parameter_names") = default_params;
  return params;
}

REFACTOR_PowerDegradationFunction::REFACTOR_PowerDegradationFunction(
    const InputParameters & parameters)
  : REFACTOR_DegradationFunctionBase(parameters)
{
}
