//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ExplicitAccelAux.h"

registerMooseObject("raccoonApp", ExplicitAccelAux);

InputParameters
ExplicitAccelAux::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription(
      "computes the second time derivative using a central-difference time-integrator");
  params.addRequiredCoupledVar("displacement", "displacement variable");
  return params;
}

ExplicitAccelAux::ExplicitAccelAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _disp_older(coupledValueOlder("displacement")),
    _disp_old(coupledValueOld("displacement")),
    _disp(coupledValue("displacement"))
{
}

Real
ExplicitAccelAux::computeValue()
{
  return (_disp[_qp] - _disp_old[_qp] * 2.0 + _disp_older[_qp]) / (_dt * _dt);
}
