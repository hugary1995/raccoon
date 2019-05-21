//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#ifdef LIBMESH_HAVE_EXTERNAL_BOOST
#include "GammaRandomFieldFromBasisFunctions.h"
registerMooseObject("raccoonApp", GammaRandomFieldFromBasisFunctions);

template <>
InputParameters
validParams<GammaRandomFieldFromBasisFunctions>()
{
  InputParameters params = validParams<GaussianRandomFieldFromBasisFunctions>();
  params.addClassDescription("This function computes Sum_over_i(w_i * functions_i) and transforms "
                             "it into the target distribution");
  params.addRequiredParam<Real>("mean", "average value of the field");
  params.addRequiredParam<Real>("CV", "standard derivation of the field");
  return params;
}

GammaRandomFieldFromBasisFunctions::GammaRandomFieldFromBasisFunctions(
    const InputParameters & parameters)
  : GaussianRandomFieldFromBasisFunctions(parameters),
    _k(1.0 / getParam<Real>("CV") / getParam<Real>("CV")),
    _theta(getParam<Real>("mean") / _k)
{
}

Real
GammaRandomFieldFromBasisFunctions::value(Real t, const Point & p)
{
  Real val = GaussianRandomFieldFromBasisFunctions::value(t, p);
  return GaussianToGamma(val);
}

Real
GammaRandomFieldFromBasisFunctions::GaussianToGamma(Real val)
{
  // get cdf of val
  Real x = 0.5 * boost::math::erfc(-val / std::sqrt(2.0));
  // convert cdf to Gamma pdf
  Real g = boost::math::gamma_p_inv(_k, x) * _theta;
  return g;
}
#endif
