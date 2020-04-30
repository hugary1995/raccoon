//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Transient.h"

class TransientSubcycling : public Transient
{
public:
  static InputParameters validParams();

  TransientSubcycling(const InputParameters & parameters);
  virtual Real relativeSolutionDifferenceNorm() override;
};
