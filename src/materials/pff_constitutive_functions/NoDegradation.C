//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "NoDegradation.h"

registerMooseObject("raccoonApp", NoDegradation);

InputParameters
NoDegradation::validParams()
{
  InputParameters params = DegradationFunctionBase::validParams();
  params.addClassDescription("Dummy function for no degradation, i.e. $g = 1$.");

  params.set<std::string>("expression") = "1";

  return params;
}

NoDegradation::NoDegradation(const InputParameters & parameters)
  : DegradationFunctionBase(parameters)
{
}
