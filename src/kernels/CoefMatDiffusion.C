//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CoefMatDiffusion.h"

registerADMooseObject("raccoonApp", CoefMatDiffusion);

defineADLegacyParams(CoefMatDiffusion);

template <ComputeStage compute_stage>
InputParameters
CoefMatDiffusion<compute_stage>::validParams()
{
  InputParameters params = ADKernelGrad<compute_stage>::validParams();
  params.addClassDescription(
      "Reaction term optionally multiplied with a coefficient and a material property");
  params.addParam<Real>("coefficient", 1.0, "Coefficient of the term");
  params.addParam<MaterialPropertyName>("material_property_name",
                                        "name of the material property to provide the multiplier");
  return params;
}

template <ComputeStage compute_stage>
CoefMatDiffusion<compute_stage>::CoefMatDiffusion(const InputParameters & parameters)
  : ADKernelGrad<compute_stage>(parameters),
    _coef(getParam<Real>("coefficient")),
    _prop(isParamValid("material_property_name")
              ? &getADMaterialProperty<Real>("material_property_name")
              : NULL)
{
}

template <ComputeStage compute_stage>
ADRealVectorValue
CoefMatDiffusion<compute_stage>::precomputeQpResidual()
{
  ADReal factor = _coef * (_prop ? (*_prop)[_qp] : 1.0);
  return factor * _grad_u[_qp];
}
