//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CrackSurfaceDensity.h"

registerADMooseObject("raccoonApp", CrackSurfaceDensity);

defineADLegacyParams(CrackSurfaceDensity);

template <ComputeStage compute_stage>
InputParameters
CrackSurfaceDensity<compute_stage>::validParams()
{
  InputParameters params = ADMaterial<compute_stage>::validParams();
  params.addClassDescription("computes the crack surface density as a function of damage");
  params.addRequiredCoupledVar("d", "damage variable");
  params.addRequiredParam<FunctionName>("local_dissipation_norm",
                                        "norm of the local dissipation ||w(d)||");
  params.addParam<MaterialPropertyName>(
      "local_dissipation_name",
      "w",
      "name of the material that holds the local dissipation function");
  params.addParam<MaterialPropertyName>("crack_surface_density_name",
                                        "gamma",
                                        "name of the material to store the crack surface density");
  params.addParam<MaterialPropertyName>(
      "crack_surface_normal_name",
      "n",
      "name of the material to store the crack surface normal, $\\grad d / \\norm{\\grad d}$");

  return params;
}

template <ComputeStage compute_stage>
CrackSurfaceDensity<compute_stage>::CrackSurfaceDensity(const InputParameters & parameters)
  : ADMaterial<compute_stage>(parameters),
    _c0(getFunction("local_dissipation_norm")),
    _L(getMaterialProperty<Real>("phase_field_regularization_length")),
    _grad_d(adCoupledGradient("d")),
    _w(getADMaterialProperty<Real>("local_dissipation_name")),
    _gamma(declareADProperty<Real>(getParam<MaterialPropertyName>("crack_surface_density_name"))),
    _n(declareADProperty<RealVectorValue>(
        getParam<MaterialPropertyName>("crack_surface_normal_name")))
{
}

template <ComputeStage compute_stage>
void
CrackSurfaceDensity<compute_stage>::computeQpProperties()
{
  Real c0 = _c0.value(_t, _q_point[_qp]);
  _gamma[_qp] = 1.0 / c0 / _L[_qp] * (_w[_qp] + _L[_qp] * _L[_qp] * _grad_d[_qp] * _grad_d[_qp]);

  if (_grad_d[_qp].norm() > 0.0)
    _n[_qp] = _grad_d[_qp] / _grad_d[_qp].norm();
  else
    _n[_qp] = _grad_d[_qp];
}
