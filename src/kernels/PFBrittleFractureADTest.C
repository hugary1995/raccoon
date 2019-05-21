//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PFBrittleFractureADTest.h"

registerADMooseObject("raccoonApp", PFBrittleFractureADTest);

defineADValidParams(
    PFBrittleFractureADTest,
    ADKernel,
    params.addParam<MaterialPropertyName>("kappa_name", "kappa", "kappa name");
    params.addParam<MaterialPropertyName>("mobility_name", "M", "name of mobility");
    params.addParam<MaterialPropertyName>("degradation_name", "g", "name of degradation");
    params.addParam<MaterialPropertyName>("elastic_energy_name", "E_el", "elastic energy name"););

template <ComputeStage compute_stage>
PFBrittleFractureADTest<compute_stage>::PFBrittleFractureADTest(const InputParameters & parameters)
  : ADKernel<compute_stage>(parameters),
    _kappa(adGetMaterialProperty<Real>("kappa_name")),
    _M(adGetMaterialProperty<Real>("mobility_name")),
    _g_name(adGetParam<MaterialPropertyName>("degradation_name")),
    _dg_dd(adGetADMaterialProperty<Real>(propertyNameFirst(_g_name, _var.name()))),
    _E_el_pos_old(adGetMaterialPropertyOld<Real>("elastic_energy_name"))
{
}

template <ComputeStage compute_stage>
ADResidual
PFBrittleFractureADTest<compute_stage>::computeQpResidual()
{
  // diffusion
  ADReal residual_diffusion = _kappa[_qp] * _grad_test[_i][_qp] * _grad_u[_qp];

  // reaction like driving force
  ADReal residual_driving =
      _test[_i][_qp] * (_dg_dd[_qp] * _E_el_pos_old[_qp] / _M[_qp] + 2.0 * _u[_qp]);

  return residual_diffusion + residual_driving;
}

template class PFBrittleFractureADTest<RESIDUAL>;
template class PFBrittleFractureADTest<JACOBIAN>;
