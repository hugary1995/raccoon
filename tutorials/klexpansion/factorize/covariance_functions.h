//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "libmesh/libmesh.h"
#include "newton_raphson.h"
#include "struve0.h"
#include <boost/math/special_functions/bessel.hpp>

class PSE : public NewtonRaphson
{
public:
  PSE(Real L, Real p) : _L(L), _p(p) { _c = solve(0); }

  Real covariance(Real tau)
  {
    Real phi = std::pow(std::sin(M_PI * tau / _p), 2) / _L / _L;
    return std::exp(-_c * phi);
  }

protected:
  virtual Real residual(Real x) override
  {
    Real coef = x / 2 / _L / _L;
    return 0.5 * _p * std::exp(-coef) * boost::math::cyl_bessel_i<int, Real>(0, coef) - _L;
  }

  virtual Real jacobian(Real x) override
  {
    Real coef = x / 2 / _L / _L;
    return 0.25 / _L / _L * _p * std::exp(-coef) *
           (boost::math::cyl_bessel_i<int, Real>(1, coef) -
            boost::math::cyl_bessel_i<int, Real>(0, coef));
  }

private:
  Real _L;
  Real _p;
  Real _c;
};

class PE : public NewtonRaphson
{
public:
  PE(Real L, Real p) : _L(L), _p(p) { _c = solve(0); }

  Real covariance(Real tau)
  {
    Real phi = std::abs(std::sin(M_PI * tau / _p)) / _L;
    return std::exp(-_c * phi);
  }

protected:
  virtual Real residual(Real x) override
  {
    return 0.5 * _p * (boost::math::cyl_bessel_i<int, Real>(0, x / _L) - struve0(x / _L)) - _L;
  }

  virtual Real jacobian(Real x) override
  {
    return 0.5 * _p / _L * (boost::math::cyl_bessel_i<int, Real>(1, x / _L) - dstruve0(x / _L));
  }

private:
  Real _L;
  Real _p;
  Real _c;
};

Real
covariance_sqexp(const Point & p1, const Point & p2, Real L)
{
  // R = exp(-pi*tau^2/(4*L^2))
  Point dist = p1 - p2;
  Real tau = dist.norm();
  return std::exp(-M_PI * tau * tau / 4 / L / L);
}

Real
covariance_exp(const Point & p1, const Point & p2, Real L)
{
  // R = exp(-tau/L)
  Point dist = p1 - p2;
  Real tau = dist.norm();
  return std::exp(-tau / L);
}
