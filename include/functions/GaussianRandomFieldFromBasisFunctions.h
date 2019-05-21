//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Function.h"
#include "FunctionInterface.h"

class GaussianRandomFieldFromBasisFunctions;

template <>
InputParameters validParams<GaussianRandomFieldFromBasisFunctions>();

class GaussianRandomFieldFromBasisFunctions : public Function, protected FunctionInterface
{
public:
  GaussianRandomFieldFromBasisFunctions(const InputParameters & parameters);

  virtual Real value(Real t, const Point & pt) override;

private:
  std::vector<Real> _w;
  std::vector<Function *> _f;
};

