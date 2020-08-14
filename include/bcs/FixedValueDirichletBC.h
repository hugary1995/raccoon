//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "DirichletBCBase.h"

class FixedValueDirichletBC : public DirichletBCBase
{
public:
  static InputParameters validParams();

  FixedValueDirichletBC(const InputParameters & parameters);

protected:
  virtual Real computeQpValue() override;

  /// The value for this BC
  const VariableValue & _u_old;
};
