//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PhaseFieldFractureContactKKT.h"

registerADMooseObject("raccoonApp", PhaseFieldFractureContactKKT);

defineADLegacyParams(PhaseFieldFractureContactKKT);

template <ComputeStage compute_stage>
InputParameters
PhaseFieldFractureContactKKT<compute_stage>::validParams()
{
  InputParameters params = ADKernelValue<compute_stage>::validParams();
  params.addParam<MaterialPropertyName>("penetration_name",
                                        "penetration",
                                        "name of the material property to provide the penetration");
  return params;
}

template <ComputeStage compute_stage>
PhaseFieldFractureContactKKT<compute_stage>::PhaseFieldFractureContactKKT(
    const InputParameters & parameters)
  : ADKernelValue<compute_stage>(parameters),
    _penetration(getADMaterialProperty<Real>("penetration_name"))
{
}

template <ComputeStage compute_stage>
ADReal
PhaseFieldFractureContactKKT<compute_stage>::precomputeQpResidual()
{
  return _u[_qp] - _penetration[_qp] -
         std::sqrt(_u[_qp] * _u[_qp] + _penetration[_qp] * _penetration[_qp] + 1e-6);
}
