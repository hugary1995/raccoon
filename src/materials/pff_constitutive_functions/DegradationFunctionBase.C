//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "DegradationFunctionBase.h"

InputParameters
DegradationFunctionBase::validParams()
{
  InputParameters params = CustomParsedFunctionBase::validParams();

  params.set<std::string>("property_name") = "g";
  params.addRequiredCoupledVar("phase_field", "The phase-field variable");

  params.set<unsigned int>("derivative_order") = 1;
  params.suppressParameter<unsigned int>("derivative_order");
  return params;
}

DegradationFunctionBase::DegradationFunctionBase(const InputParameters & parameters)
  : CustomParsedFunctionBase(parameters)
{
}
