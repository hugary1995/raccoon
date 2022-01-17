//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "JIntegralDomainFormPressureWork.h"

registerMooseObject("raccoonApp", JIntegralDomainFormPressureWork);

InputParameters
JIntegralDomainFormPressureWork::validParams()
{
  InputParameters params = ElementIntegralPostprocessor::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription(
      "This class computes the correction term in the domain form J-integral when the regularized "
      "crack surface is not traction-free. In particular, this class accounts for pressure acting "
      "on the crack surfaces.");
  params.addRequiredCoupledVar("phase_field", "The phase-field variable");
  params.addRequiredParam<MaterialPropertyName>("pressure", "Material property name for pressure");
  params.addParam<MaterialPropertyName>("indicator_function",
                                        "I"
                                        "The indicator function");
  params.addRequiredCoupledVar(
      "displacements",
      "The displacements appropriate for the simulation geometry and coordinate system");
  params.addRequiredParam<RealVectorValue>("J_direction", "direction of J integral");
  params.addRequiredCoupledVar(
      "domain",
      "The domain indication variable q. q = 1 inside the domain, q = 0 outside the domain.");
  return params;
}

JIntegralDomainFormPressureWork::JIntegralDomainFormPressureWork(const InputParameters & parameters)
  : ElementIntegralPostprocessor(parameters),
    BaseNameInterface(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _integral_value_old(getPostprocessorValueOldByName(name())),
    _grad_d(coupledGradient("phase_field")),
    _p(getADMaterialProperty<Real>(prependBaseName("pressure", true))),
    _dI_dd(getADMaterialProperty<Real>(derivativePropertyNameFirst(
        getParam<MaterialPropertyName>("indicator_function"), getVar("phase_field", 0)->name()))),
    _ndisp(coupledComponents("displacements")),
    _grad_u(coupledGradients("displacements")),
    _t(getParam<RealVectorValue>("J_direction")),
    _q(coupledValue("domain"))
{
  for (unsigned int i = _ndisp; i < 3; ++i)
    _grad_u.push_back(&_grad_zero);
}

Real
JIntegralDomainFormPressureWork::computeQpIntegral()
{
  RankTwoTensor H((*_grad_u[0])[_qp], (*_grad_u[1])[_qp], (*_grad_u[2])[_qp]);
  return -raw_value(_p[_qp] * _t * H * _grad_d[_qp] * _dI_dd[_qp] * _q[_qp]);
}
