//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "REFACTOR_RationalDegradationFunction.h"

registerMooseObject("raccoonApp", REFACTOR_RationalDegradationFunction);

InputParameters
REFACTOR_RationalDegradationFunction::validParams()
{
  InputParameters params = REFACTOR_DegradationFunctionBase::validParams();
  params.addClassDescription(
      "Defines the rational degradation function $g(d) = "
      "\\frac{(1-d)^p}{(1-d)^p+a_1d(1+a_2d+a_2a_3d^2)} (1-\\eta) + \\eta$ where $a_1 = "
      "\\frac{\\Gc}{\\psi_c}\\frac{\\xi}{c_0l}$.");

  params.set<std::string>("function") =
      "(1-d)^p/((1-d)^p+(Gc/psic*xi/c0/l)*d*(1+a2*d+a2*a3*d^2))*(1-eta)+eta";

  const std::vector<std::string> default_params = {"p", "a2", "a3", "eta"};
  params.set<std::vector<std::string>>("parameter_names") = default_params;

  const std::vector<std::string> default_mat_props = {"Gc", "psic", "xi", "c0", "l"};
  params.set<std::vector<std::string>>("material_property_names") = default_mat_props;
  return params;
}

REFACTOR_RationalDegradationFunction::REFACTOR_RationalDegradationFunction(
    const InputParameters & parameters)
  : REFACTOR_DegradationFunctionBase(parameters)
{
}
