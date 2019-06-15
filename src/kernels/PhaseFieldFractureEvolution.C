//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PhaseFieldFractureEvolution.h"

registerADMooseObject("raccoonApp", PhaseFieldFractureEvolution);

defineADValidParams(
    PhaseFieldFractureEvolution,
    ADKernel,
    params.addParam<MaterialPropertyName>("kappa_name", "kappa", "kappa name");
    params.addParam<MaterialPropertyName>("mobility_name", "M", "name of mobility");
    params.addParam<MaterialPropertyName>("degradation_name", "g", "name of degradation");
    params.addParam<MaterialPropertyName>("local_dissipation_name",
                                          "w",
                                          "name of local dissipation");
    params.addParam<MaterialPropertyName>("driving_energy_name", "name of the driving energy");
    params.addCoupledVar("driving_energy_var", "auxiliary variable that holds the driving energy");
    params.addParam<bool>(
        "lag_driving_energy",
        false,
        "whether we should use last step's driving energy to improve convergence"););

template <ComputeStage compute_stage>
PhaseFieldFractureEvolution<compute_stage>::PhaseFieldFractureEvolution(
    const InputParameters & parameters)
  : ADKernel<compute_stage>(parameters),
    _kappa(adGetMaterialProperty<Real>("kappa_name")),
    _M(adGetMaterialProperty<Real>("mobility_name")),
    _g_name(adGetParam<MaterialPropertyName>("degradation_name")),
    _dg_dd(adGetADMaterialProperty<Real>(derivativePropertyNameFirst(_g_name, _var.name()))),
    _w_name(adGetParam<MaterialPropertyName>("local_dissipation_name")),
    _dw_dd(adGetADMaterialProperty<Real>(derivativePropertyNameFirst(_w_name, _var.name()))),
    _lag(adGetParam<bool>("lag_driving_energy")),
    _D_material_coupled(isParamValid("driving_energy_name")),
    _D_variable_coupled(isParamValid("driving_energy_var")),
    _D_mat(nullptr),
    _D_mat_old(nullptr),
    _D_var(nullptr)
{
  if (_D_variable_coupled && !_D_material_coupled)
    _D_var = &coupledValue("driving_energy_var");
  else if (!_D_variable_coupled && _D_material_coupled)
    if (_lag)
      _D_mat_old = &adGetMaterialPropertyOld<Real>("driving_energy_name");
    else
      _D_mat = &adGetADMaterialProperty<Real>("driving_energy_name");
  else
    mooseError("Driving energy should be either provided as a material property or an auxiliary "
               "variable, not both");
}
template <ComputeStage compute_stage>
ADResidual
PhaseFieldFractureEvolution<compute_stage>::computeQpResidual()
{
  ADReal D;
  if (_D_var)
    D = (*_D_var)[_qp];
  else if (_D_mat)
    D = (*_D_mat)[_qp];
  else if (_D_mat_old)
    D = (*_D_mat_old)[_qp];
  else
    mooseError("PhaseFieldFractureEvolution: Internal Error");

  // diffusion
  ADReal residual_diffusion = _M[_qp] * _kappa[_qp] * _grad_test[_i][_qp] * _grad_u[_qp];

  // reaction like driving force
  ADReal residual_driving = _test[_i][_qp] * (_dg_dd[_qp] * D + _dw_dd[_qp] * _M[_qp]);

  return residual_diffusion + residual_driving;
}
