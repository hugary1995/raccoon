//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "REFACTOR_CrackGeometricFunction.h"

registerMooseObject("raccoonApp", REFACTOR_CrackGeometricFunction);

InputParameters
REFACTOR_CrackGeometricFunction::validParams()
{
  InputParameters params = REFACTOR_CustomParsedFunctionBase::validParams();

  params.set<std::string>("f_name") = "alpha";
  params.addRequiredCoupledVar("d", "the phase-field variable");

  params.set<unsigned int>("derivative_order") = 1;
  params.suppressParameter<unsigned int>("derivative_order");

  params.addParam<MaterialPropertyName>("initial_derivative_name",
                                        "dalpha/dd(0)",
                                        "Name of the material to store the initial slope of the "
                                        "crack geometric function, $\\alpha(d=0)$");

  params.addParam<MaterialPropertyName>("normalization_constant_name",
                                        "c0",
                                        "Name of the material to store the normalization constant, "
                                        "$4\\int_0^1 \\sqrt{\\alpha(s)} \\diff{s}$");

  params.addParam<Real>(
      "tolerance", 1e-8, "Tolerance of the numerically computed normalization constant");
  params.addParam<unsigned int>("maximum_iterations",
                                1e9,
                                "Maximum number of iterations allowed for the numerical "
                                "computation of the normalization constant");
  return params;
}

REFACTOR_CrackGeometricFunction::REFACTOR_CrackGeometricFunction(const InputParameters & parameters)
  : REFACTOR_CustomParsedFunctionBase(parameters),
    _d_idx(argIndex(coupled("d"))),
    _xi(declareADProperty<Real>(getParam<MaterialPropertyName>("initial_derivative_name"))),
    _c0(declareADProperty<Real>(getParam<MaterialPropertyName>("normalization_constant_name"))),
    _tolerance(getParam<Real>("tolerance")),
    _max_its(getParam<unsigned int>("maximum_iterations"))
{
  // set d to 0 and evaluate the first derivative of the crack geometric function
  _func_params[_d_idx] = 0;
  _xi_0 = evaluate(_derivatives[0]._F, _name);
  _c0_0 = computeNormalizationConstant();
}

ADReal
REFACTOR_CrackGeometricFunction::computeNormalizationConstant()
{
  // We use an adaptive trapezoidal rule to integrate the normalization constant to a target
  // precision
  std::stack<std::pair<Real, Real>> S;
  S.emplace(0, 1);
  ADReal I = 0;
  unsigned int its = 0;
  while (!S.empty() && its < _max_its)
  {
    auto interval = S.top();
    S.pop();

    Real a = interval.first;
    Real b = interval.second;
    Real m = (a + b) / 2;

    ADReal I1 = (normalizationIntegrand(a) + normalizationIntegrand(b)) * (b - a) / 2;
    ADReal I2 =
        (normalizationIntegrand(a) + 2 * normalizationIntegrand(m) + normalizationIntegrand(b)) *
        (b - a) / 4;
    if (std::abs(I1 - I2) < 3 * (b - a) * _tolerance)
      I += I2;
    else
    {
      S.emplace(a, m);
      S.emplace(m, b);
      its++;
    }
  }
  if (its == _max_its)
    mooseError("Maximum number of iterations reached, but the crack geometric function still "
               "hasn't converge.");
  return I;
}

ADReal
REFACTOR_CrackGeometricFunction::normalizationIntegrand(ADReal d)
{
  _func_params[_d_idx] = d;
  return 4.0 * std::sqrt(evaluate(_func_F, _name));
}

void
REFACTOR_CrackGeometricFunction::computeQpProperties()
{
  _xi[_qp] = _xi_0;
  _c0[_qp] = _c0_0;

  REFACTOR_CustomParsedFunctionBase::computeQpProperties();
}
