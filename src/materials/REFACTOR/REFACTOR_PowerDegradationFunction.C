//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

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
