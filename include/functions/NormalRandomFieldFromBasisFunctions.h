#ifndef NormalRandomFieldFromBasisFunctions_H
#define NormalRandomFieldFromBasisFunctions_H

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

#endif // NormalRandomFieldFromBasisFunctions_H
