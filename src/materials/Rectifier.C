//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "Rectifier.h"

registerMooseObject("raccoonApp", Rectifier);

InputParameters
Rectifier::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription("This class computes the crack opening displacement.");

  params.addRequiredCoupledVar("phase_field", "The phase-field variable");
  params.addRequiredParam<Real>("small_D", "smaller D");
  params.addRequiredParam<Real>("large_D", "larger D");
  params.addParam<MaterialPropertyName>("diffusivity", "D", "larger D");

  return params;
}

Rectifier::Rectifier(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _D(declareProperty<RankTwoTensor>(prependBaseName("diffusivity", true))),
    _d(coupledValue("phase_field")),
    _grad_d(coupledGradient("phase_field")),
    _D_small(getParam<Real>("small_D")),
    _D_large(getParam<Real>("large_D"))
{
}

void
Rectifier::computeQpProperties()
{
  RankTwoTensor I(RankTwoTensor::initIdentity);

  RankTwoTensor nn;
  if (_grad_d[_qp].norm() > 1e-6)
  {
    RealVectorValue n = _grad_d[_qp] / _grad_d[_qp].norm();
    nn.vectorOuterProduct(n, n);
    _D[_qp] = I * _D_small + nn * _D_large;
  }
  else
    _D[_qp] = I * _D_small;
}
