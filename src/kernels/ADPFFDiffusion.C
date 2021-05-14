//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADPFFDiffusion.h"
#include "Assembly.h"

registerMooseObject("raccoonApp", ADPFFDiffusion);

InputParameters
ADPFFDiffusion::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addClassDescription("Compute the diffusion term in phase-field evolution equation");
  params.addParam<MaterialPropertyName>(
      "fracture_toughness", "Gc", "The fracture toughness $\\Gc$");
  params.addParam<MaterialPropertyName>(
      "normalization_constant", "c0", "The normalization constant $c_0$");
  params.addParam<MaterialPropertyName>(
      "regularization_length", "l", "The phase-field regularization length");
  return params;
}

ADPFFDiffusion::ADPFFDiffusion(const InputParameters & parameters)
  : ADKernel(parameters),
    _Gc(getADMaterialProperty<Real>("fracture_toughness")),
    _c0(getADMaterialProperty<Real>("normalization_constant")),
    _l(getADMaterialProperty<Real>("regularization_length")),
    _coord_sys(_assembly.coordSystem())
{
}

ADReal
ADPFFDiffusion::computeQpResidual()
{
  ADReal value = _grad_test[_i][_qp] * _grad_u[_qp];

  if (_coord_sys == Moose::COORD_RZ)
    value -= _test[_i][_qp] / _ad_q_point[_qp](0) * _grad_u[_qp](0);

  return 2 * _Gc[_qp] * _l[_qp] / _c0[_qp] * value;
}
