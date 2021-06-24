//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "EffectivePressure.h"

registerMooseObject("raccoonApp", EffectivePressure);

InputParameters
EffectivePressure::validParams()
{
  InputParameters params = ElementIntegralPostprocessor::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription("This class computes the effective pressure of the form $\\int_\\body "
                             "p \\norm{\\grad d} \\diff{V}$.");

  params.addRequiredParam<MaterialPropertyName>("pressure", "Material property name for pressure");
  params.addRequiredCoupledVar("phase_field", "The phase-field variable");
  params.addRequiredParam<MaterialPropertyName>("indicator_function",
                                                "I"
                                                "The pressure indicator function");
  return params;
}

EffectivePressure::EffectivePressure(const InputParameters & parameters)
  : ElementIntegralPostprocessor(parameters),
    BaseNameInterface(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _p(getADMaterialProperty<Real>(prependBaseName("pressure", true))),
    _grad_d(coupledGradient("phase_field")),
    _d_name(getVar("phase_field", 0)->name()),
    _I_name(prependBaseName("indicator_function", true)),
    _dI_dd(getADMaterialProperty<Real>(derivativePropertyNameFirst(_I_name, _d_name)))
{
}

Real
EffectivePressure::computeQpIntegral()
{
  return raw_value(_p[_qp]) * _grad_d[_qp].norm() * raw_value(_dI_dd[_qp]);
}
