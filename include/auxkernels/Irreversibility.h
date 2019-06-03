//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "AuxKernel.h"

// Forward Declarations
class Irreversibility;

template <>
InputParameters validParams<Irreversibility>();

class Irreversibility : public AuxKernel
{
public:
  Irreversibility(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;

  NumericVector<Number> & _upper_vector;
  NumericVector<Number> & _lower_vector;
  unsigned _bounded_var_num;
  const VariableValue & _lower_bound;
  const Real _upper_bound;
};
