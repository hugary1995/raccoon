//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "FPIMaterialPropertyAux.h"
#include "metaphysicl/raw_type.h"
#include "raccoonAppTypes.h"

registerMooseObject("raccoonApp", FPIMaterialPropertyAux);
registerMooseObject("raccoonApp", ADFPIMaterialPropertyAux);

template <bool is_ad>
InputParameters
FPIMaterialPropertyAuxTempl<is_ad>::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("reads a material property into a monomial.");
  params.addRequiredParam<MaterialPropertyName>("from", "material to read the value from");

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
FPIMaterialPropertyAuxTempl<is_ad>::FPIMaterialPropertyAuxTempl(const InputParameters & parameters)
  : AuxKernel(parameters), _v(getGenericMaterialProperty<Real, is_ad>("from"))
{
}

template <bool is_ad>
Real
FPIMaterialPropertyAuxTempl<is_ad>::computeValue()
{
  return MetaPhysicL::raw_value(_v[_qp]);
}

template class FPIMaterialPropertyAuxTempl<false>;
template class FPIMaterialPropertyAuxTempl<true>;
