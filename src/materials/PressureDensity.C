//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PressureDensity.h"

registerMooseObject("raccoonApp", PressureDensity);

InputParameters
PressureDensity::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription("This class computes the effective pressure density $p \\norm{\\grad "
                             "d} I(d)$, where $I(d)$ is the indicator function.");

  params.addParam<MaterialPropertyName>(
      "effective_pressure_density", "p_density", "Name of the pressure density");

  params.addRequiredCoupledVar("phase_field", "The phase-field variable");
  params.addRequiredParam<MaterialPropertyName>("pressure", "Material property name for pressure");
  params.addParam<MaterialPropertyName>("indicator_function",
                                        "I"
                                        "The indicator function");

  return params;
}

PressureDensity::PressureDensity(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _p_density(declareADProperty<Real>(prependBaseName("effective_pressure_density", true))),
    _grad_d(adCoupledGradient("phase_field")),
    _p(getADMaterialProperty<Real>(prependBaseName("pressure", true))),
    _dI_dd(getADMaterialProperty<Real>(derivativePropertyNameFirst(
        getParam<MaterialPropertyName>("indicator_function"), getVar("phase_field", 0)->name())))
{
}

void
PressureDensity::computeQpProperties()
{
  _p_density[_qp] = _p[_qp] * _grad_d[_qp].norm() * _dI_dd[_qp];
}
