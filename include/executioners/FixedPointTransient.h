//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Transient.h"

#include "FixedPointSolve.h"

class FixedPointTransient : public Transient
{
public:
  static InputParameters validParams();

  FixedPointTransient(const InputParameters & parameters);

  virtual void init() override;

protected:
  FixedPointSolve _fp_solve;
};
