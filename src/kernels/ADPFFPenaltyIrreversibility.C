//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADPFFPenaltyIrreversibility.h"

registerMooseObject("raccoonApp", ADPFFPenaltyIrreversibility);

InputParameters
ADPFFPenaltyIrreversibility::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  params.addClassDescription(
      "Quadratic penalty enforcing phase-field irreversibility. The weak form is "
      "$(w, \\gamma \\langle d - d_{old} \\rangle_-)$, with "
      "$\\langle x \\rangle_- = \\min(x,0)$, penalizing any decrease of the phase field "
      "below its previous converged value. The penalty coefficient is "
      "$\\gamma = (G_c/l)(1/\\tau_{irr}^2 - 1)$.");
  params.addRequiredParam<Real>(
      "penalty", "The penalty coefficient gamma = (Gc/l)(1/tau_irr^2 - 1).");
  return params;
}

ADPFFPenaltyIrreversibility::ADPFFPenaltyIrreversibility(const InputParameters & parameters)
  : ADKernelValue(parameters), _penalty(getParam<Real>("penalty")), _d_old(_var.slnOld())
{
}

ADReal
ADPFFPenaltyIrreversibility::precomputeQpResidual()
{
  // gamma * <d - d_old>_-  with  <x>_- = min(x, 0).
  // AD yields the Heaviside Jacobian  gamma * H^-(d - d_old)  automatically.
  const ADReal delta = _u[_qp] - _d_old[_qp];
  return delta < 0.0 ? _penalty * delta : ADReal(0.0);
}
