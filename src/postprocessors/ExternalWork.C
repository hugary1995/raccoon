//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ExternalWork.h"

registerMooseObject("raccoonApp", ExternalWork);

InputParameters
ExternalWork::validParams()
{
  InputParameters params = NodalPostprocessor::validParams();
  params.addClassDescription("Compute the increase in external energy during the current step");
  params.addRequiredCoupledVar("forces",
                               "The reaction forces associated with each of the displacement");
  params.addRequiredCoupledVar(
      "displacements",
      "The displacements appropriate for the simulation geometry and coordinate system");
  return params;
}

ExternalWork::ExternalWork(const InputParameters & parameters)
  : NodalPostprocessor(parameters),
    _sum(0),
    _ndisp(coupledComponents("displacements")),
    _disp_0(coupledValue("displacements", 0)),
    _disp_1(_ndisp >= 2 ? coupledValue("displacements", 1) : _zero),
    _disp_2(_ndisp >= 3 ? coupledValue("displacements", 2) : _zero),
    _disp_0_old(coupledValueOld("displacements", 0)),
    _disp_1_old(_ndisp >= 2 ? coupledValueOld("displacements", 1) : _zero),
    _disp_2_old(_ndisp >= 3 ? coupledValueOld("displacements", 2) : _zero),
    _nforce(coupledComponents("forces")),
    _f_0(coupledValue("forces", 0)),
    _f_1(_ndisp >= 2 ? coupledValue("forces", 1) : _zero),
    _f_2(_ndisp >= 3 ? coupledValue("forces", 2) : _zero),
    _f_0_old(coupledValueOld("forces", 0)),
    _f_1_old(_ndisp >= 2 ? coupledValueOld("forces", 1) : _zero),
    _f_2_old(_ndisp >= 3 ? coupledValueOld("forces", 2) : _zero),
    _sum_old(getPostprocessorValueOldByName(name()))
{
  if (_ndisp != _nforce)
    mooseError("number of displacements mush match number of forces");
}

void
ExternalWork::initialize()
{
  _sum = 0;
}

void
ExternalWork::execute()
{
  _sum += computeQpValue();
}

Real
ExternalWork::computeQpValue()
{
  RealVectorValue u(_disp_0[_qp], _disp_1[_qp], _disp_2[_qp]);
  RealVectorValue u_old(_disp_0_old[_qp], _disp_1_old[_qp], _disp_2_old[_qp]);

  RealVectorValue f(_f_0[_qp], _f_1[_qp], _f_2[_qp]);
  RealVectorValue f_old(_f_0_old[_qp], _f_1_old[_qp], _f_2_old[_qp]);

  return 0.5 * (f + f_old) * (u - u_old);
}

Real
ExternalWork::getValue()
{
  gatherSum(_sum);
  return _sum + _sum_old;
}

void
ExternalWork::threadJoin(const UserObject & y)
{
  const ExternalWork & pps = static_cast<const ExternalWork &>(y);
  _sum += pps._sum;
}
