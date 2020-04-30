//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "IntegratedBC.h"

class CoupledNeumannBC : public IntegratedBC
{
public:
  static InputParameters validParams();

  CoupledNeumannBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;

private:
  Real _coef;
  const VariableValue & _v;
};
