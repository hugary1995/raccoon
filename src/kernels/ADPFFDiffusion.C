//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADPFFDiffusion.h"
#include "Assembly.h"

registerADMooseObject("raccoonApp", ADPFFDiffusion);

InputParameters
ADPFFDiffusion::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addClassDescription("computes the diffusion term in phase-field evolution equation");
  params.addParam<MaterialPropertyName>("kappa_name", "kappa", "kappa name");
  params.addParam<MaterialPropertyName>("mobility_name", "mobility", "name of mobility");
  return params;
}

ADPFFDiffusion::ADPFFDiffusion(
    const InputParameters & parameters)
  : ADKernel(parameters),
    _kappa(getMaterialProperty<Real>("kappa_name")),
    _M(getADMaterialProperty<Real>("mobility_name")),
    _coord_sys(_assembly.coordSystem())
{
}

ADReal
ADPFFDiffusion::computeQpResidual()
{
  ADReal residual =
      _grad_test[_i][_qp](0) * _grad_u[_qp](0) + _grad_test[_i][_qp](1) * _grad_u[_qp](1);
  if (_coord_sys == Moose::COORD_RZ)
    residual -= _test[_i][_qp] / _ad_q_point[_qp](0) * _grad_u[_qp](0);
  else
    residual += _grad_test[_i][_qp](2) * _grad_u[_qp](2);

  return _M[_qp] * _kappa[_qp] * residual;
}
