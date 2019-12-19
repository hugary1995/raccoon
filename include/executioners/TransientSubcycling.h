//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Transient.h"

// Forward Declarations
class TransientSubcycling;

template <>
InputParameters validParams<TransientSubcycling>();

class TransientSubcycling : public Transient
{
public:
  TransientSubcycling(const InputParameters & parameters);
  virtual Real relativeSolutionDifferenceNorm() override;
};
