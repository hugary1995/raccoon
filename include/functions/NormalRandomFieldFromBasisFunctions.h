//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "GaussianRandomFieldFromBasisFunctions.h"

class NormalRandomFieldFromBasisFunctions;

template <>
InputParameters validParams<NormalRandomFieldFromBasisFunctions>();

class NormalRandomFieldFromBasisFunctions : public GaussianRandomFieldFromBasisFunctions
{
public:
  NormalRandomFieldFromBasisFunctions(const InputParameters & parameters);

  virtual Real value(Real t, const Point & pt) override;

private:
  // transform from a Gaussian RV to a Gamma RV
  virtual Real GaussianToNormal(Real);

  // mean
  Real _mu;
  // standard deviation
  Real _sigma;
};

