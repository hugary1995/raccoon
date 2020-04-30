//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "raccoonAppTypes.h"
#include "FPIMaterialPropertyUserObject.h"

registerMooseObject("raccoonApp", FPIMaterialPropertyUserObject);

InputParameters
FPIMaterialPropertyUserObject::validParams()
{
  InputParameters params = MaterialPropertyUserObject::validParams();

  // add EXEC_BETWEEN_FPI to available execution flags
  ExecFlagEnum & exec = params.set<ExecFlagEnum>("execute_on");
  exec.addAvailableFlags(EXEC_BETWEEN_FPI);
  params.setDocString("execute_on", exec.getDocString());

  // set execution flags and prevent users from modifying them
  params.set<ExecFlagEnum>("execute_on") = {EXEC_INITIAL, EXEC_BETWEEN_FPI, EXEC_TIMESTEP_END};
  params.suppressParameter<ExecFlagEnum>("execute_on");

  return params;
}

FPIMaterialPropertyUserObject::FPIMaterialPropertyUserObject(const InputParameters & parameters)
  : MaterialPropertyUserObject(parameters)
{
}
