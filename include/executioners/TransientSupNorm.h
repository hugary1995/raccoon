//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Transient.h"

// Forward Declarations
class TransientSupNorm;

template <>
InputParameters validParams<TransientSupNorm>();

class TransientSupNorm : public Transient
{
public:
  TransientSupNorm(const InputParameters & parameters);
  virtual Real relativeSolutionDifferenceNorm() override;
};
