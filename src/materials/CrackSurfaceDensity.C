//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CrackSurfaceDensity.h"

registerMooseObject("raccoonApp", CrackSurfaceDensity);

InputParameters
CrackSurfaceDensity::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription(
      "This class computes the crack surface density $\\gamma = \\dfrac{1}{c_0 l} (\\alpha + l^2 "
      "\\grad d \\cdot \\grad d)$, where $\\alpha$ is the crack geometric function, $c_0$ is the "
      "normalization constant, and $l$ is the phase-field regularization length.");
  params.addParam<std::string>("base_name",
                               "Optional parameter that allows the user to define "
                               "multiple mechanics material systems on the same "
                               "block, i.e. for multiple phases");

  params.addRequiredCoupledVar("phase_field", "The phase-field variable");
  params.addParam<MaterialPropertyName>(
      "normalization_constant", "c0", "The normalization constant");
  params.addParam<MaterialPropertyName>(
      "regularization_length", "l", "The phase-field regularization length");
  params.addParam<MaterialPropertyName>(
      "crack_geometric_function", "alpha", "The crack geometric function");
  params.addParam<MaterialPropertyName>(
      "crack_surface_density", "gamma", "Name of the crack surface density");

  return params;
}

CrackSurfaceDensity::CrackSurfaceDensity(const InputParameters & parameters)
  : Material(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _grad_d(adCoupledGradient("phase_field")),
    _c0(getADMaterialProperty<Real>(_base_name +
                                    getParam<MaterialPropertyName>("normalization_constant"))),
    _l(getADMaterialProperty<Real>(_base_name +
                                   getParam<MaterialPropertyName>("regularization_length"))),
    _alpha(getADMaterialProperty<Real>(_base_name +
                                       getParam<MaterialPropertyName>("crack_geometric_function"))),
    _gamma(declareADProperty<Real>(_base_name +
                                   getParam<MaterialPropertyName>("crack_surface_density")))
{
}

void
CrackSurfaceDensity::computeQpProperties()
{
  _gamma[_qp] =
      (_alpha[_qp] + _l[_qp] * _l[_qp] * _grad_d[_qp] * _grad_d[_qp]) / _c0[_qp] / _l[_qp];
}
