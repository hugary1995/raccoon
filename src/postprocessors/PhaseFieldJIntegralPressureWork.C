//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PhaseFieldJIntegralPressureWork.h"

registerMooseObject("raccoonApp", PhaseFieldJIntegralPressureWork);

InputParameters
PhaseFieldJIntegralPressureWork::validParams()
{
  InputParameters params = ElementIntegralPostprocessor::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription("This class computes the total external work done by pressure.");
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

PhaseFieldJIntegralPressureWork::PhaseFieldJIntegralPressureWork(const InputParameters & parameters)
  : ElementIntegralPostprocessor(parameters),
    BaseNameInterface(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _integral_value_old(getPostprocessorValueOldByName(name())),
    _grad_d(coupledGradient("phase_field")),
    _p(getADMaterialProperty<Real>(prependBaseName("pressure", true))),
    _dI_dd(getADMaterialProperty<Real>(derivativePropertyNameFirst(
        getParam<MaterialPropertyName>("indicator_function"), getVar("phase_field", 0)->name()))),
    _ndisp(coupledComponents("displacements")),
    _u_dots(coupledDots("displacements")),
    _t(getParam<RealVectorValue>("J_direction")),
    _grad_q(coupledGradient("domain"))
{
  for (unsigned int i = _ndisp; i < 3; ++i)
    _u_dots.push_back(&_zero);
}

Real
PhaseFieldJIntegralPressureWork::computeQpIntegral()
{
  RealVectorValue u_dot((*_u_dots[0])[_qp], (*_u_dots[1])[_qp], (*_u_dots[2])[_qp]);
  return -raw_value(_p[_qp] * _grad_d[_qp] * _dI_dd[_qp] * u_dot * _t * _grad_q[_qp]);
}

Real
PhaseFieldJIntegralPressureWork::getValue()
{
  gatherSum(_integral_value);
  return _integral_value * _dt + _integral_value_old;
}
