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
    params.addRequiredParam<MaterialPropertyName>("driving_energy_name",
                                                  "name of the driving energy");
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
    _D(adGetParam<bool>("lag_driving_energy")
           ? adGetMaterialPropertyOld<Real>("driving_energy_name")
           : adGetADMaterialProperty<Real>("driving_energy_name"))
{
}

template <ComputeStage compute_stage>
ADResidual
PhaseFieldFractureEvolution<compute_stage>::computeQpResidual()
{
  // diffusion
  ADReal residual_diffusion = _M[_qp] * _kappa[_qp] * _grad_test[_i][_qp] * _grad_u[_qp];

  // reaction like driving force
  ADReal residual_driving = _test[_i][_qp] * (_dg_dd[_qp] * _D[_qp] + _dw_dd[_qp] * _M[_qp]);

  return residual_diffusion + residual_driving;
}

template class PhaseFieldFractureEvolution<RESIDUAL>;
template class PhaseFieldFractureEvolution<JACOBIAN>;
