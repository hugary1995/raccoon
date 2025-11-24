//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ArrheniusLaw.h"

registerMooseObject("raccoonApp", ArrheniusLaw);

InputParameters
ArrheniusLaw::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription("This class computes the Arrhenius coefficient $\\exp\\left( "
                             "-\\dfrac{Q}{RT} \\right)$, where $Q$ is the activation energy, $R$ "
                             "is the ideal gas constant, and $T$ is the temperature.");

  params.addParam<MaterialPropertyName>(
      "arrhenius_coefficient", "A", "Name of the Arrhenius coefficient material");
  params.addParam<MaterialPropertyName>("activation_energy", "Q", "The activation energy");
  params.addRequiredParam<Real>("ideal_gas_constant", "The ideal gas constant");
  params.addRequiredCoupledVar("temperature", "The temperature");

  return params;
}

ArrheniusLaw::ArrheniusLaw(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _arrhenius_coef_name(prependBaseName("arrhenius_coefficient", true)),
    _arrhenius_coef(declareADProperty<Real>(_arrhenius_coef_name)),
    _darrhenius_coef_dT(declareADProperty<Real>(
        derivativePropertyName(_arrhenius_coef_name, {getVar("temperature", 0)->name()}))),
    _Q(getADMaterialProperty<Real>(prependBaseName("activation_energy", true))),
    _R(getParam<Real>("ideal_gas_constant")),
    _T(adCoupledValue("temperature"))
{
}

void
ArrheniusLaw::computeQpProperties()
{
  using std::exp;
  _arrhenius_coef[_qp] = exp(-_Q[_qp] / _R / _T[_qp]);
  _darrhenius_coef_dT[_qp] = _arrhenius_coef[_qp] * _Q[_qp] / _R / _T[_qp] / _T[_qp];
}
