//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CoefMatSource.h"

registerADMooseObject("raccoonApp", CoefMatSource);

defineADLegacyParams(CoefMatSource);

template <ComputeStage compute_stage>
InputParameters
CoefMatSource<compute_stage>::validParams()
{
  InputParameters params = ADKernelValue<compute_stage>::validParams();
  params.addClassDescription(
      "Reaction term optionally multiplied with a coefficient and a material property");
  params.addParam<Real>("coefficient", 1.0, "Coefficient of the term");
  params.addParam<std::vector<MaterialPropertyName>>(
      "prop_names", "names of the material properties to provide the multiplier");
  return params;
}

template <ComputeStage compute_stage>
CoefMatSource<compute_stage>::CoefMatSource(const InputParameters & parameters)
  : ADKernelValue<compute_stage>(parameters),
    _coef(getParam<Real>("coefficient")),
    _prop_names(getParam<std::vector<MaterialPropertyName>>("prop_names")),
    _num_props(_prop_names.size())
{
  _props.resize(_num_props);
  for (unsigned int i = 0; i < _num_props; i++)
    _props[i] = &getADMaterialProperty<Real>(_prop_names[i]);
}

template <ComputeStage compute_stage>
ADReal
CoefMatSource<compute_stage>::precomputeQpResidual()
{
  ADReal factor = 1.0;
  for (auto prop : _props)
    factor *= (*prop)[_qp];

  return factor * _coef;
}
