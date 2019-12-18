//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PhaseFieldFractureEvolutionBase.h"

defineADLegacyParams(PhaseFieldFractureEvolutionBase);

template <ComputeStage compute_stage>
InputParameters
PhaseFieldFractureEvolutionBase<compute_stage>::validParams()
{
  InputParameters params = ADKernel<compute_stage>::validParams();
  params.addParam<MaterialPropertyName>("kappa_name", "kappa", "kappa name");
  params.addParam<MaterialPropertyName>("mobility_name", "M", "name of mobility");
  params.addParam<MaterialPropertyName>("degradation_name", "g", "name of degradation");
  params.addParam<MaterialPropertyName>("local_dissipation_name", "w", "name of local dissipation");
  params.addParam<MaterialPropertyName>("driving_energy_name", "name of the driving energy");
  params.addCoupledVar("driving_energy_var", "auxiliary variable that holds the driving energy");
  params.addParam<bool>("lag_driving_energy",
                        false,
                        "whether we should use last step's driving energy to improve convergence");
  return params;
}

template <ComputeStage compute_stage>
PhaseFieldFractureEvolutionBase<compute_stage>::PhaseFieldFractureEvolutionBase(
    const InputParameters & parameters)
  : ADKernel<compute_stage>(parameters),
    _kappa(getMaterialProperty<Real>("kappa_name")),
    _M(getMaterialProperty<Real>("mobility_name")),
    _g_name(getParam<MaterialPropertyName>("degradation_name")),
    _dg_dd(getADMaterialProperty<Real>(derivativePropertyNameFirst(_g_name, _var.name()))),
    _w_name(getParam<MaterialPropertyName>("local_dissipation_name")),
    _dw_dd(getADMaterialProperty<Real>(derivativePropertyNameFirst(_w_name, _var.name()))),
    _lag(getParam<bool>("lag_driving_energy")),
    _D_material_coupled(isParamValid("driving_energy_name")),
    _D_variable_coupled(isParamValid("driving_energy_var")),
    _D_mat(nullptr),
    _D_mat_old(nullptr),
    _D_var(nullptr)
{
  if (_D_variable_coupled && !_D_material_coupled)
    _D_var = &adCoupledValue("driving_energy_var");
  else if (!_D_variable_coupled && _D_material_coupled)
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
PhaseFieldFractureEvolutionBase<compute_stage>::fractureDrivingEnergy()
{
  ADReal D;
  if (_D_var)
    D = (*_D_var)[_qp];
  else if (_D_mat)
    D = (*_D_mat)[_qp];
  else if (_D_mat_old)
    D = (*_D_mat_old)[_qp];
  else
    mooseError("PhaseFieldFractureEvolutionBase: Internal Error");

  return D;
}

adBaseClass(PhaseFieldFractureEvolutionBase);
