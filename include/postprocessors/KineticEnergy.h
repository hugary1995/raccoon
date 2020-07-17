//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElementIntegralPostprocessor.h"

class KineticEnergy : public ElementIntegralPostprocessor
{
public:
  static InputParameters validParams();

  KineticEnergy(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral() override;
  const MaterialProperty<Real> & _rho;
  const unsigned _ndisp;
  std::vector<const ADVariableValue *> _vel_var;
};
