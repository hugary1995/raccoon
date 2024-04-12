//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "RationalDegradationFunction.h"

registerMooseObject("raccoonApp", RationalDegradationFunction);

InputParameters
RationalDegradationFunction::validParams()
{
  InputParameters params = DegradationFunctionBase::validParams();
  params.addClassDescription(
      "Defines the rational degradation function $g(d) = "
      "\\frac{(1-d)^p}{(1-d)^p+a_1d(1+a_2d+a_2a_3d^2)} (1-\\eta) + \\eta$ where $a_1 = "
      "\\frac{\\Gc}{\\psi_c}\\frac{\\xi}{c_0l}$. $\\Gc$ is the fracture toughness, $\\psi_c$ is "
      "the critical fracture energy, $\\xi$ is the derivative of the local fracture energy at "
      "$\\d=0$, $c_0$ is the normalization constant, and $l$ is the regularization length.");

  params.set<std::string>("expression") =
      "(1-d)^p/((1-d)^p+(Gc/psic*xi/c0/l)*d*(1+a2*d+a2*a3*d^2))*(1-eta)+eta";
  params.set<std::vector<std::string>>("parameter_names") = {"p", "a2", "a3", "eta"};
  params.set<std::vector<std::string>>("material_property_names") = {"Gc", "psic", "xi", "c0", "l"};
  return params;
}

RationalDegradationFunction::RationalDegradationFunction(const InputParameters & parameters)
  : DegradationFunctionBase(parameters)
{
}
