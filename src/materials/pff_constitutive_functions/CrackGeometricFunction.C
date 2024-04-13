//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CrackGeometricFunction.h"

registerMooseObject("raccoonApp", CrackGeometricFunction);

InputParameters
CrackGeometricFunction::validParams()
{
  InputParameters params = CustomParsedFunctionBase::validParams();
  params.addClassDescription(
      "This is a wrapper of ADDerivativeParsedMaterial to conveniently define a crack geometric "
      "function. The initial derivative as well as the normalization constant are automatically "
      "populated given the function definition.");

  params.set<std::string>("property_name") = "alpha";
  params.addRequiredCoupledVar("phase_field", "The phase-field variable");

  params.set<unsigned int>("derivative_order") = 1;
  params.suppressParameter<unsigned int>("derivative_order");

  params.addParam<MaterialPropertyName>("initial_derivative",
                                        "xi",
                                        "Name of the material to store the initial slope of the "
                                        "crack geometric function, $\\alpha(d=0)$");

  params.addParam<MaterialPropertyName>("normalization_constant",
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

CrackGeometricFunction::CrackGeometricFunction(const InputParameters & parameters)
  : CustomParsedFunctionBase(parameters),
    _d_idx(argIndex(coupled("phase_field"))),
    _xi(declareADProperty<Real>(getParam<MaterialPropertyName>("initial_derivative"))),
    _c0(declareADProperty<Real>(getParam<MaterialPropertyName>("normalization_constant"))),
    _tolerance(getParam<Real>("tolerance")),
    _max_its(getParam<unsigned int>("maximum_iterations"))
{
  // set d to 0 and evaluate the first derivative of the crack geometric function
  _func_params[_d_idx] = 0;
  _xi_0 = evaluate(_derivatives[0]._F, _name);
  _c0_0 = computeNormalizationConstant();
}

ADReal
CrackGeometricFunction::computeNormalizationConstant()
{
  // We use an adaptive trapezoidal rule to integrate the normalization constant to a target
  // precision
  std::stack<std::pair<Real, Real>> S;
  S.emplace(0, 1);
  ADReal I = 0;
  unsigned int its = 0;
  while (!S.empty())
  {
    auto interval = S.top();
    S.pop();

    Real a = interval.first;
    Real b = interval.second;
    Real m = (a + b) / 2;

    Real I1 = (normalizationIntegrand(a) + normalizationIntegrand(b)) * (b - a) / 2;
    Real I2 =
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

    if (its >= _max_its)
      mooseError("Maximum number of iterations reached, but the crack geometric function still "
                 "hasn't converge.");
  }
  return I;
}

Real
CrackGeometricFunction::normalizationIntegrand(const ADReal & d)
{
  _func_params[_d_idx] = d;
  return 4.0 * std::sqrt(raw_value(evaluate(_func_F, _name)));
}

void
CrackGeometricFunction::computeQpProperties()
{
  _xi[_qp] = _xi_0;
  _c0[_qp] = _c0_0;

  CustomParsedFunctionBase::computeQpProperties();
}
