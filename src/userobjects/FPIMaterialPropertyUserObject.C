//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "raccoonAppTypes.h"
#include "FPIMaterialPropertyUserObject.h"

registerMooseObject("raccoonApp", FPIMaterialPropertyUserObject);
registerMooseObject("raccoonApp", ADFPIMaterialPropertyUserObject);

template <bool is_ad>
InputParameters
FPIMaterialPropertyUserObjectTempl<is_ad>::validParams()
{
  InputParameters params = MaterialPropertyUserObjectTempl<is_ad>::validParams();

  // add EXEC_BETWEEN_FPI to available execution flags
  ExecFlagEnum & exec = params.set<ExecFlagEnum>("execute_on");
  exec.addAvailableFlags(EXEC_BETWEEN_FPI);
  params.setDocString("execute_on", exec.getDocString());

  // set execution flags and prevent users from modifying them
  params.set<ExecFlagEnum>("execute_on") = {EXEC_INITIAL, EXEC_BETWEEN_FPI, EXEC_TIMESTEP_END};
  params.suppressParameter<ExecFlagEnum>("execute_on");

  return params;
}

template <bool is_ad>
FPIMaterialPropertyUserObjectTempl<is_ad>::FPIMaterialPropertyUserObjectTempl(
    const InputParameters & parameters)
  : MaterialPropertyUserObjectTempl<is_ad>(parameters)
{
}

template class FPIMaterialPropertyUserObjectTempl<false>;
template class FPIMaterialPropertyUserObjectTempl<true>;
