//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "AuxKernel.h"

// Forward Declarations
class DamageRelaxation;

template <>
InputParameters validParams<DamageRelaxation>();

class DamageRelaxation : public AuxKernel
{
public:
  DamageRelaxation(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;
  const VariableValue & _d;
  const VariableValue & _d_prev_nl;
};
