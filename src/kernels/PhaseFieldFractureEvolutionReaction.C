//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PhaseFieldFractureEvolutionReaction.h"

registerADMooseObject("raccoonApp", PhaseFieldFractureEvolutionReaction);

defineADLegacyParams(PhaseFieldFractureEvolutionReaction);

template <ComputeStage compute_stage>
InputParameters
PhaseFieldFractureEvolutionReaction<compute_stage>::validParams()
{
  InputParameters params = ADKernelValue<compute_stage>::validParams();
  params.addClassDescription("computes the reaction term in phase-field evolution equation");
  params.addParam<MaterialPropertyName>("mobility_name", "M", "name of mobility");
  params.addParam<MaterialPropertyName>("degradation_name", "g", "name of degradation");
  params.addParam<MaterialPropertyName>("local_dissipation_name", "w", "name of local dissipation");
  params.addParam<MaterialPropertyName>("driving_energy_name", "name of the driving energy");
  params.addCoupledVar("driving_energy_var", "auxiliary variable that holds the driving energy");
  params.addParam<bool>(
      "lag", false, "whether we should use last step's driving energy to improve convergence");
  return params;
}

template <ComputeStage compute_stage>
PhaseFieldFractureEvolutionReaction<compute_stage>::PhaseFieldFractureEvolutionReaction(
    const InputParameters & parameters)
  : ADKernelValue<compute_stage>(parameters),
    _M(getMaterialProperty<Real>("mobility_name")),
    _dg_dd(getADMaterialProperty<Real>(derivativePropertyNameFirst(
        getParam<MaterialPropertyName>("degradation_name"), _var.name()))),
    _dw_dd(getADMaterialProperty<Real>(derivativePropertyNameFirst(
        getParam<MaterialPropertyName>("local_dissipation_name"), _var.name()))),
    _lag(getParam<bool>("lag")),
    _D_mat(nullptr),
    _D_mat_old(nullptr),
    _D_var(nullptr)
{
  if (isParamValid("driving_energy_var") && !isParamValid("driving_energy_name"))
    _D_var = &adCoupledValue("driving_energy_var");
  else if (!isParamValid("driving_energy_var") && isParamValid("driving_energy_name"))
    if (_lag)
      _D_mat_old = &getMaterialPropertyOld<Real>("driving_energy_name");
    else
      _D_mat = &getADMaterialProperty<Real>("driving_energy_name");
  else
    mooseError("Driving energy should be either provided as a material property or an auxiliary "
               "variable, not both");
}

template <ComputeStage compute_stage>
ADReal
PhaseFieldFractureEvolutionReaction<compute_stage>::precomputeQpResidual()
{
  ADReal D;
  if (_D_var)
    D = (*_D_var)[_qp];
  else if (_D_mat)
    D = (*_D_mat)[_qp];
  else if (_D_mat_old)
    D = (*_D_mat_old)[_qp];
  else
    mooseError("PhaseFieldFractureEvolutionReaction: Internal Error");

  // reaction like driving force
  return _dg_dd[_qp] * D + _dw_dd[_qp] * _M[_qp];
}
