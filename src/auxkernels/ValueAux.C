//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ValueAux.h"

registerMooseObject("raccoonApp", ValueAux);

template <>
InputParameters
validParams<ValueAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("coupled_variable",
                               "transfer value from the coupled variable to this aux variable");
  return params;
}

ValueAux::ValueAux(const InputParameters & parameters)
  : AuxKernel(parameters), _v(coupledValue("coupled_variable"))
{
}

Real
ValueAux::computeValue()
{
  return _v[_qp];
}
