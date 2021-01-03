//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElementIntegralPostprocessor.h"

class PhaseFieldPressure : public ElementIntegralPostprocessor
{
public:
  static InputParameters validParams();

  PhaseFieldPressure(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral() override;
  const VariableGradient & _grad_d;
  const ADMaterialProperty<Real> & _p_mat;
};
