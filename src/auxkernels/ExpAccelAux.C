//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ExpAccelAux.h"

registerMooseObject("raccoonApp", ExpAccelAux);

InputParameters
ExpAccelAux::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addRequiredCoupledVar("displacement", "displacement variable");
  return params;
}

ExpAccelAux::ExpAccelAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _disp_older(coupledValueOlder("displacement")),
    _disp_old(coupledValueOld("displacement")),
    _disp(coupledValue("displacement"))
{
}

Real
ExpAccelAux::computeValue()
{
  return (_disp[_qp] - _disp_old[_qp] * 2.0 + _disp_older[_qp]) / (_dt * _dt);
}
