//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "libmesh/libmesh.h"

Real
struve0(Real x)
{
  Real T0 = 2 * x / M_PI;
  unsigned int k = 0;
  Real Tk = T0;
  Real L = Tk;

  while (Tk > T0 * 1e-10)
  {
    Tk *= 4 * x * x * (k + 2) * (k + 2) / (2 * k + 3) / (2 * k + 3) / (2 * k + 4) / (2 * k + 4);
    L += Tk;
    k++;
  }

  return L;
}

Real
dstruve0(Real x)
{
  Real T0 = 2 / M_PI;
  unsigned int k = 0;
  Real Tk = T0;
  Real L = Tk;

  while (Tk > T0 * 1e-10)
  {
    Tk *= 4 * x * x * (k + 1) * (k + 2) / (2 * k + 1) / (2 * k + 2) / (2 * k + 3) / (2 * k + 4);
    L += Tk;
    k++;
  }

  return L;
}
