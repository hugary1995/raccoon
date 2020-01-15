//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADComputePlaneSmallStrain.h"

registerADMooseObject("raccoonApp", ADComputePlaneSmallStrain);

defineADLegacyParams(ADComputePlaneSmallStrain);

template <ComputeStage compute_stage>
InputParameters
ADComputePlaneSmallStrain<compute_stage>::validParams()
{
  InputParameters params = ADCompute2DSmallStrain<compute_stage>::validParams();
  params.addClassDescription("Compute a small strain under plane stress assumption");
  params.addCoupledVar("out_of_plane_strain", "Nonlinear variable for plane stress condition");
  return params;
}

template <ComputeStage compute_stage>
ADComputePlaneSmallStrain<compute_stage>::ADComputePlaneSmallStrain(
    const InputParameters & parameters)
  : ADCompute2DSmallStrain<compute_stage>(parameters),
    _out_of_plane_strain(adCoupledValue("out_of_plane_strain"))
{
}

template <ComputeStage compute_stage>
ADReal
ADComputePlaneSmallStrain<compute_stage>::computeOutOfPlaneStrain()
{
  return _out_of_plane_strain[_qp];
}
