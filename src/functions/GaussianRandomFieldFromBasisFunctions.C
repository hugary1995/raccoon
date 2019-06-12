//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "GaussianRandomFieldFromBasisFunctions.h"

registerMooseObject("raccoonApp", GaussianRandomFieldFromBasisFunctions);

template <>
InputParameters
validParams<GaussianRandomFieldFromBasisFunctions>()
{
  InputParameters params = validParams<Function>();
  params.addClassDescription("This function computes Sum_over_i(w_i * functions_i) and transforms "
                             "it into the target distribution");
  params.addRequiredParam<std::vector<FunctionName>>("functions", "list of basis functions");
  params.addRequiredParam<std::vector<Real>>(
      "w", "a vector of Gaussian weights. Number of weights must equal number of basis functions");
  return params;
}

GaussianRandomFieldFromBasisFunctions::GaussianRandomFieldFromBasisFunctions(
    const InputParameters & parameters)
  : Function(parameters), FunctionInterface(this), _w(getParam<std::vector<Real>>("w"))
{
  const std::vector<FunctionName> & names = getParam<std::vector<FunctionName>>("functions");
  const unsigned int len = names.size();
  if (len != _w.size())
    mooseError("GaussianRandomFieldFromBasisFunctions: The number of basis functions must equal "
               "the number "
               "of w values");

  _f.resize(len);
  for (unsigned i = 0; i < len; ++i)
  {
    if (name() == names[i])
      mooseError("A GaussianRandomFieldFromBasisFunctions must not reference itself");
    const Function * f = &getFunctionByName(names[i]);
    if (!f)
      mooseError("GaussianRandomFieldFromBasisFunctions: The basis function ",
                 names[i],
                 " (referenced by ",
                 name(),
                 ") cannot be found");
    _f[i] = f;
  }
}

Real
GaussianRandomFieldFromBasisFunctions::value(Real t, const Point & p) const
{
  Real val = 0;
  for (unsigned i = 0; i < _f.size(); ++i)
    val += _w[i] * _f[i]->value(t, p);
  return val;
}
