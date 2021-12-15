//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PhaseFieldJIntegralDomainForm.h"

registerMooseObject("raccoonApp", PhaseFieldJIntegralDomainForm);

InputParameters
PhaseFieldJIntegralDomainForm::validParams()
{
  InputParameters params = ElementIntegralPostprocessor::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription("This class computes the total external work done by pressure.");
  params.addRequiredCoupledVar(
      "displacements",
      "The displacements appropriate for the simulation geometry and coordinate system");
  params.addRequiredParam<RealVectorValue>("J_direction", "direction of J integral");
  params.addParam<MaterialPropertyName>("strain_energy_density",
                                        "psie"
                                        "Name of the strain energy density");
  params.addRequiredCoupledVar(
      "domain",
      "The domain indication variable q. q = 1 inside the domain, q = 0 outside the domain.");
  return params;
}

PhaseFieldJIntegralDomainForm::PhaseFieldJIntegralDomainForm(const InputParameters & parameters)
  : ElementIntegralPostprocessor(parameters),
    BaseNameInterface(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _stress(getADMaterialPropertyByName<RankTwoTensor>(prependBaseName("stress"))),
    _psie(getADMaterialProperty<Real>(prependBaseName("strain_energy_density"))),
    _ndisp(coupledComponents("displacements")),
    _grad_u(coupledGradients("displacements")),
    _t(getParam<RealVectorValue>("J_direction")),
    _grad_q(coupledGradient("domain"))
{
  for (unsigned int i = _ndisp; i < 3; ++i)
    _grad_u.push_back(&_grad_zero);
}

Real
PhaseFieldJIntegralDomainForm::computeQpIntegral()
{
  RankTwoTensor H((*_grad_u[0])[_qp], (*_grad_u[1])[_qp], (*_grad_u[2])[_qp]);
  RankTwoTensor I2(RankTwoTensor::initIdentity);
  ADRankTwoTensor Sigma = _psie[_qp] * I2 - H.transpose() * _stress[_qp];
  return -raw_value(_t * Sigma * _grad_q[_qp]);
}
