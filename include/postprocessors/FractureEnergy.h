//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElementIntegralPostprocessor.h"

class FractureEnergy : public ElementIntegralPostprocessor
{
public:
  static InputParameters validParams();

  FractureEnergy(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral() override;
  const MaterialProperty<Real> & _kappa;
  const MaterialProperty<Real> & _M;
  const VariableGradient & _grad_d;
  const MaterialProperty<Real> & _w;
};
