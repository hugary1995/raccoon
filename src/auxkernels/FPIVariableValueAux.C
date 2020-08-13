//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "FPIVariableValueAux.h"
#include "metaphysicl/raw_type.h"
#include "raccoonAppTypes.h"

registerMooseObject("raccoonApp", FPIVariableValueAux);
registerMooseObject("raccoonApp", ADFPIVariableValueAux);

template <bool is_ad>
InputParameters
FPIVariableValueAuxTempl<is_ad>::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("reads the coupled variable value into the aux variable.");
  params.addRequiredCoupledVar("from", "variable to read the value from");

  // add EXEC_BETWEEN_FPI to available execution flags
  ExecFlagEnum & exec = params.set<ExecFlagEnum>("execute_on");
  exec.addAvailableFlags(EXEC_BETWEEN_FPI);
  params.setDocString("execute_on", exec.getDocString());

  // set execution flags and prevent users from modifying them
  params.set<ExecFlagEnum>("execute_on") = {EXEC_BETWEEN_FPI, EXEC_TIMESTEP_BEGIN};
  params.suppressParameter<ExecFlagEnum>("execute_on");

  return params;
}

template <bool is_ad>
FPIVariableValueAuxTempl<is_ad>::FPIVariableValueAuxTempl(const InputParameters & parameters)
  : AuxKernel(parameters), _v(coupledGenericValue<is_ad>("from"))
{
}

template <bool is_ad>
Real
FPIVariableValueAuxTempl<is_ad>::computeValue()
{
  return MetaPhysicL::raw_value(_v[_qp]);
}

template class FPIVariableValueAuxTempl<false>;
template class FPIVariableValueAuxTempl<true>;
