//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElementIntegralPostprocessor.h"

class CrackVolume;

template <>
InputParameters validParams<CrackVolume>();

class CrackVolume : public ElementIntegralPostprocessor
{
public:
  CrackVolume(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral() override;
  const VariableValue & _disp_x;
  const VariableValue & _disp_y;
  const VariableGradient & _grad_d;
};
