//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

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
