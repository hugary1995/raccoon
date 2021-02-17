//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

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
