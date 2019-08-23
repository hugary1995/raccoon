//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "AuxKernel.h"

// Forward Declarations
class DamageExtrapolation;

template <>
InputParameters validParams<DamageExtrapolation>();

class DamageExtrapolation : public AuxKernel
{
public:
  DamageExtrapolation(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;
  const VariableValue & _d;
  const VariableValue & _d_old;
  const VariableValue & _d_older;
};
