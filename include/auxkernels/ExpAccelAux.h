//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "AuxKernel.h"

class ExpAccelAux;

template <>
InputParameters validParams<ExpAccelAux>();

class ExpAccelAux : public AuxKernel
{
public:
  ExpAccelAux(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;

  const VariableValue & _disp_older;
  const VariableValue & _disp_old;
  const VariableValue & _disp;
};
