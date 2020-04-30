//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "FPIVariableValueAux.h"
#include "raccoonAppTypes.h"

registerMooseObject("raccoonApp", FPIVariableValueAux);

InputParameters
FPIVariableValueAux::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("reads the coupled variable value into the aux variable.");
  params.addRequiredCoupledVar("from", "variable to read the value from");

  // add EXEC_BETWEEN_FPI to available execution flags
  ExecFlagEnum & exec = params.set<ExecFlagEnum>("execute_on");
  exec.addAvailableFlags(EXEC_BETWEEN_FPI);
  params.setDocString("execute_on", exec.getDocString());

  // set execution flags and prevent users from modifying them
  params.set<ExecFlagEnum>("execute_on") = {EXEC_BETWEEN_FPI, EXEC_TIMESTEP_END};
  params.suppressParameter<ExecFlagEnum>("execute_on");

  return params;
}

FPIVariableValueAux::FPIVariableValueAux(const InputParameters & parameters)
  : AuxKernel(parameters), _v(coupledValue("from"))
{
}

Real
FPIVariableValueAux::computeValue()
{
  return _v[_qp];
}
