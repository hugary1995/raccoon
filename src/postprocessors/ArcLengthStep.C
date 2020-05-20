//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ArcLengthStep.h"
#include "raccoonAppTypes.h"

registerMooseObject("raccoonApp", ArcLengthStep);

InputParameters
ArcLengthStep::validParams()
{
  InputParameters params = NodalPostprocessor::validParams();
  params.addClassDescription(
      "Compute the arclength step size such that a fixed amount of energy is dissipated");
  params.addRequiredCoupledVar("forces",
                               "The reaction forces associated with each of the displacement");
  params.addRequiredCoupledVar(
      "displacements",
      "The displacements appropriate for the simulation geometry and coordinate system");
  params.addRequiredParam<Real>("delta_D", "the target increment of dissipation per step");
  params.addRequiredParam<Real>("delta_U", "the target increment of internal energy per step");

  // add EXEC_BETWEEN_FPI to available execution flags
  ExecFlagEnum & exec = params.set<ExecFlagEnum>("execute_on");
  exec.addAvailableFlags(EXEC_BETWEEN_FPI);
  params.setDocString("execute_on", exec.getDocString());

  // set execution flags and prevent users from modifying them
  params.set<ExecFlagEnum>("execute_on") = {EXEC_BETWEEN_FPI, EXEC_TIMESTEP_END};
  params.suppressParameter<ExecFlagEnum>("execute_on");

  return params;
}

ArcLengthStep::ArcLengthStep(const InputParameters & parameters)
  : NodalPostprocessor(parameters),
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
    _delta_D(getParam<Real>("delta_D")),
    _delta_U(getParam<Real>("delta_U")),
    _sum1(0),
    _sum2(0),
    _lambda_old(getPostprocessorValueOldByName(name()))
{
  if (_ndisp != _nforce)
    mooseError("number of displacements must match number of forces");
}

void
ArcLengthStep::initialize()
{
  _sum1 = 0;
  _sum2 = 0;
}

void
ArcLengthStep::execute()
{
  RealVectorValue u(_disp_0[_qp], _disp_1[_qp], _disp_2[_qp]);
  RealVectorValue u_old(_disp_0_old[_qp], _disp_1_old[_qp], _disp_2_old[_qp]);
  RealVectorValue f(_f_0[_qp], _f_1[_qp], _f_2[_qp]);
  _sum1 += u * f;
  _sum2 += u_old * f;
}

Real
ArcLengthStep::getValue()
{
  if (_t_step == 1)
    return 1;
  gatherSum(_sum1);
  gatherSum(_sum2);
  if (MooseUtils::absoluteFuzzyEqual(_sum2, 0))
    return (_lambda_old * _sum2 + 2 * _delta_U) / _sum1;
  else
    return (_lambda_old * _sum1 - 2 * _delta_D) / _sum2;
}

void
ArcLengthStep::threadJoin(const UserObject & y)
{
  const ArcLengthStep & pps = static_cast<const ArcLengthStep &>(y);
  _sum1 += pps._sum1;
  _sum2 += pps._sum2;
}
