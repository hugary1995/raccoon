//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "libmesh/libmesh.h"

class NewtonRaphson
{
public:
  Real solve(Real initial_guess, Real TOL = 1e-10)
  {
    Real x = initial_guess;
    Real r = residual(x);
    while (std::abs(r) > TOL)
    {
      x -= r / jacobian(x);
      r = residual(x);
    }
    return x;
  }

protected:
  virtual Real residual(Real x) = 0;
  virtual Real jacobian(Real x) = 0;
};
