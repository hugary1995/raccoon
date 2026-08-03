//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADPFFViscosity.h"

registerMooseObject("raccoonApp", ADPFFViscosity);

InputParameters
ADPFFViscosity::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  params.addClassDescription(
      "Viscous (rate-dependent) regularization of the phase-field evolution. The weak form is "
      "$(w, \\eta \\dot{d})$ with $\\dot{d} \\approx (d - d_{old})/\\Delta t$, adding a viscous "
      "term to the phase-field residual that smooths abrupt softening; the rate-independent "
      "model is recovered as $\\eta \\to 0$.");
  params.addRequiredParam<Real>("viscosity", "The viscosity coefficient eta.");
  return params;
}

ADPFFViscosity::ADPFFViscosity(const InputParameters & parameters)
  : ADKernelValue(parameters), _viscosity(getParam<Real>("viscosity")), _d_old(_var.slnOld())
{
}

ADReal
ADPFFViscosity::precomputeQpResidual()
{
  // eta * d_dot with a backward-Euler rate d_dot = (d - d_old)/dt. Guard dt=0 (steady setup).
  return (_dt > 0.0) ? _viscosity * (_u[_qp] - _d_old[_qp]) / _dt : ADReal(0.0);
}
