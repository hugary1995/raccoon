//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "AD1DStressDivergenceTensors.h"

registerADMooseObject("raccoonApp", AD1DStressDivergenceTensors);

defineADLegacyParams(AD1DStressDivergenceTensors);

template <ComputeStage compute_stage>
InputParameters
AD1DStressDivergenceTensors<compute_stage>::validParams()
{
  InputParameters params = ADStressDivergenceTensors<compute_stage>::validParams();
  params.addRequiredParam<MaterialPropertyName>(
      "cross_sectional_area", "material property that provides the cross sectional area");
  return params;
}

template <ComputeStage compute_stage>
AD1DStressDivergenceTensors<compute_stage>::AD1DStressDivergenceTensors(
    const InputParameters & parameters)
  : ADStressDivergenceTensors<compute_stage>(parameters),
    _A(getADMaterialProperty<Real>("cross_sectional_area"))
{
}

template <ComputeStage compute_stage>
ADReal
AD1DStressDivergenceTensors<compute_stage>::computeQpResidual()
{
  return _A[_qp] * ADStressDivergenceTensors<compute_stage>::computeQpResidual();
}
