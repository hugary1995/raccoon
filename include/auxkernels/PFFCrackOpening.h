//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "AuxKernel.h"

class PFFCrackOpening;

class PFFCrackOpening : public AuxKernel
{
public:
  static InputParameters validParams();

  PFFCrackOpening(const InputParameters & parameters);
  virtual ~PFFCrackOpening() {}

protected:
  virtual Real computeValue();

  unsigned int _ndisp;
  std::vector<const VariableValue *> _disp;
  const VariableGradient & _grad_c;
};
