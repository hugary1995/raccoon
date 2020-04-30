//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "FPIMaterialPropertyAux.h"
#include "raccoonAppTypes.h"

registerMooseObject("raccoonApp", FPIMaterialPropertyAux);

InputParameters
FPIMaterialPropertyAux::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("reads a material property into a monomial.");
  params.addRequiredParam<MaterialPropertyName>("from", "material to read the value from");

  // add EXEC_BETWEEN_FPI to available execution flags
  ExecFlagEnum & exec = params.set<ExecFlagEnum>("execute_on");
  exec.addAvailableFlags(EXEC_BETWEEN_FPI);
  params.setDocString("execute_on", exec.getDocString());

  // set execution flags and prevent users from modifying them
  params.set<ExecFlagEnum>("execute_on") = {EXEC_BETWEEN_FPI, EXEC_TIMESTEP_END};
  params.suppressParameter<ExecFlagEnum>("execute_on");

  return params;
}

FPIMaterialPropertyAux::FPIMaterialPropertyAux(const InputParameters & parameters)
  : AuxKernel(parameters), _v(getADMaterialProperty<Real>("from"))
{
}

Real
FPIMaterialPropertyAux::computeValue()
{
  return _v[_qp].value();
}
