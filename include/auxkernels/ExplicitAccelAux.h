//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "AuxKernel.h"

class ExplicitAccelAux : public AuxKernel
{
public:
  static InputParameters validParams();

  ExplicitAccelAux(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;

  const VariableValue & _disp_older;
  const VariableValue & _disp_old;
  const VariableValue & _disp;
};
