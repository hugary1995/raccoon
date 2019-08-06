//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "DamageRelaxation.h"
#include "NonlinearSystem.h"

registerMooseObject("raccoonApp", DamageRelaxation);

template <>
InputParameters
validParams<DamageRelaxation>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("d", "damage variable to be relaxed between nonlinear iterations");
  return params;
}

DamageRelaxation::DamageRelaxation(const InputParameters & parameters)
  : AuxKernel(parameters), _d(coupledValue("d")), _d_prev_nl(coupledValuePreviousNL("d"))
{
}

Real
DamageRelaxation::computeValue()
{
  // auto nl_sys = std::dynamic_pointer_cast<NonlinearSystem>(_nl_sys);
  //
  // if (!nl_sys)
  //   mooseError("This is not a NonlinearSystem");
  //
  // unsigned int nl_its = nl_sys._current_nl_its;

  Real d_current = _d[_qp];
  Real d_prev_nl = _d_prev_nl[_qp];
  Real d_predicted = d_current;

  // if (nl_its > 1)
  d_predicted += d_current - d_prev_nl;

  d_predicted = std::min(d_predicted, 1.0);
  d_predicted = std::max(d_predicted, 0.0);

  return d_predicted;
}
