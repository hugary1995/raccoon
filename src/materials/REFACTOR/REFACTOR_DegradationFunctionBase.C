//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "REFACTOR_DegradationFunctionBase.h"

InputParameters
REFACTOR_DegradationFunctionBase::validParams()
{
  InputParameters params = REFACTOR_CustomParsedFunctionBase::validParams();

  params.set<std::string>("f_name") = "g";
  params.addRequiredCoupledVar("d", "the phase-field variable");

  params.set<unsigned int>("derivative_order") = 1;
  params.suppressParameter<unsigned int>("derivative_order");
  return params;
}

REFACTOR_DegradationFunctionBase::REFACTOR_DegradationFunctionBase(
    const InputParameters & parameters)
  : REFACTOR_CustomParsedFunctionBase(parameters)
{
}
