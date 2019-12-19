//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CoefMatReaction.h"

registerADMooseObject("raccoonApp", CoefMatReaction);

defineADLegacyParams(CoefMatReaction);

template <ComputeStage compute_stage>
InputParameters
CoefMatReaction<compute_stage>::validParams()
{
  InputParameters params = ADKernelValue<compute_stage>::validParams();
  params.addClassDescription(
      "Reaction term optionally multiplied with a coefficient and a material property");
  params.addParam<Real>("coefficient", 1.0, "Coefficient of the term");
  params.addParam<MaterialPropertyName>("material_property_name",
                                        "name of the material property to provide the multiplier");
  return params;
}

template <ComputeStage compute_stage>
CoefMatReaction<compute_stage>::CoefMatReaction(const InputParameters & parameters)
  : ADKernelValue<compute_stage>(parameters),
    _coef(getParam<Real>("coefficient")),
    _prop(isParamValid("material_property_name")
              ? &getADMaterialProperty<Real>("material_property_name")
              : NULL)
{
}

template <ComputeStage compute_stage>
ADReal
CoefMatReaction<compute_stage>::precomputeQpResidual()
{
  ADReal factor = _coef * (_prop ? (*_prop)[_qp] : 1.0);
  return factor * _u[_qp];
}
