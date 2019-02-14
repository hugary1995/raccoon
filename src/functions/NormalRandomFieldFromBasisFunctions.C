#include "NormalRandomFieldFromBasisFunctions.h"

registerMooseObject("raccoonApp", NormalRandomFieldFromBasisFunctions);

template <>
InputParameters
validParams<NormalRandomFieldFromBasisFunctions>()
{
  InputParameters params = validParams<GaussianRandomFieldFromBasisFunctions>();
  params.addClassDescription("This function computes Sum_over_i(w_i * functions_i) and transforms "
                             "it into the target distribution");
  params.addRequiredParam<Real>("mean", "average value of the field");
  params.addRequiredParam<Real>("CV", "standard derivation of the field");
  return params;
}

NormalRandomFieldFromBasisFunctions::NormalRandomFieldFromBasisFunctions(
    const InputParameters & parameters)
  : GaussianRandomFieldFromBasisFunctions(parameters),
    _mu(getParam<Real>("mean")),
    _sigma(_mu == 0.0 ? getParam<Real>("CV") : getParam<Real>("CV") * _mu)
{
}

Real
NormalRandomFieldFromBasisFunctions::value(Real t, const Point & p)
{
  Real val = GaussianRandomFieldFromBasisFunctions::value(t, p);
  return GaussianToNormal(val);
}

Real
NormalRandomFieldFromBasisFunctions::GaussianToNormal(Real val)
{
  return val * _sigma + _mu;
}
