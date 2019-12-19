//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PhaseFieldFractureEvolutionPressure.h"

registerADMooseObject("raccoonApp", PhaseFieldFractureEvolutionPressure);

defineADLegacyParams(PhaseFieldFractureEvolutionPressure);

template <ComputeStage compute_stage>
InputParameters
PhaseFieldFractureEvolutionPressure<compute_stage>::validParams()
{
  InputParameters params = ADKernelValue<compute_stage>::validParams();
  params.addClassDescription("computes the pressure term in phase-field evolution equation");
  params.addRequiredCoupledVar("p", "coupled pressure inside the crack");
  params.addRequiredCoupledVar(
      "displacements",
      "The displacements appropriate for the simulation geometry and coordinate system");
  return params;
}

template <ComputeStage compute_stage>
PhaseFieldFractureEvolutionPressure<compute_stage>::PhaseFieldFractureEvolutionPressure(
    const InputParameters & parameters)
  : ADKernelValue<compute_stage>(parameters),
    _p(adCoupledValue("p")),
    _ndisp(coupledComponents("displacements")),
    _grad_disp(3)
{
  // fetch coupled variables and gradients (as stateful properties if necessary)
  for (unsigned int i = 0; i < _ndisp; ++i)
    _grad_disp[i] = &adCoupledGradient("displacements", i);
  // set unused dimensions to zero
  for (unsigned i = _ndisp; i < 3; ++i)
    _grad_disp[i] = &adZeroGradient();
}

template <ComputeStage compute_stage>
ADReal
PhaseFieldFractureEvolutionPressure<compute_stage>::precomputeQpResidual()
{
  return -_p[_qp] *
         ((*_grad_disp[0])[_qp](0) + (*_grad_disp[1])[_qp](1) + (*_grad_disp[2])[_qp](2));
}
