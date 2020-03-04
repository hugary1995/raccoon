//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PressurizedCrack.h"

registerADMooseObject("raccoonApp", PressurizedCrack);

defineADLegacyParams(PressurizedCrack);

template <ComputeStage compute_stage>
InputParameters
PressurizedCrack<compute_stage>::validParams()
{
  InputParameters params = ADKernelValue<compute_stage>::validParams();
  params.addClassDescription("computes the body force term from pressurized phase-field fracture");
  params.addParam<MaterialPropertyName>("pressure_mat", "pressure inside the crack");
  params.addCoupledVar("pressure_var", "pressure inside the crack");
  params.addRequiredCoupledVar("d", "coupled damage variable");
  params.addRequiredParam<unsigned int>("component",
                                        "An integer corresponding to the direction "
                                        "the variable this kernel acts in. (0 for x, "
                                        "1 for y, 2 for z)");
  return params;
}

template <ComputeStage compute_stage>
PressurizedCrack<compute_stage>::PressurizedCrack(const InputParameters & parameters)
  : ADKernelValue<compute_stage>(parameters),
    _comp(getParam<unsigned int>("component")),
    _p_mat(isParamValid("pressure_mat") ? &getADMaterialProperty<Real>("pressure_mat") : nullptr),
    _p_var(isParamValid("pressure_var") ? &adCoupledValue("pressure_var") : nullptr),
    _grad_d(adCoupledGradient("d"))
{
  if (_p_mat && _p_var)
    mooseError("pressure should be provided by either pressure_mat or pressure_var, not both.");
}

template <ComputeStage compute_stage>
ADReal
PressurizedCrack<compute_stage>::precomputeQpResidual()
{
  ADReal p = _p_mat ? (*_p_mat)[_qp] : (*_p_var)[_qp];
  return p * _grad_d[_qp](_comp);
}
