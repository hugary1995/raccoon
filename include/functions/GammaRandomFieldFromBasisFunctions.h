//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once
#ifdef LIBMESH_HAVE_EXTERNAL_BOOST

#include "GaussianRandomFieldFromBasisFunctions.h"
// BOOST
#include "libmesh/ignore_warnings.h"
#include <boost/math/distributions/gamma.hpp>
#include <boost/math/special_functions/erf.hpp>
#include "libmesh/restore_warnings.h"

class GammaRandomFieldFromBasisFunctions;

template <>
InputParameters validParams<GammaRandomFieldFromBasisFunctions>();

class GammaRandomFieldFromBasisFunctions : public GaussianRandomFieldFromBasisFunctions
{
public:
  GammaRandomFieldFromBasisFunctions(const InputParameters & parameters);

  virtual Real value(Real t, const Point & pt) override;

private:
  // transform from a Gaussian RV to a Gamma RV
  virtual Real GaussianToGamma(Real);

  // shape parameter
  Real _k;
  // scale parameter
  Real _theta;
};

#endif // GammaRandomFieldFromBasisFunctions_H
