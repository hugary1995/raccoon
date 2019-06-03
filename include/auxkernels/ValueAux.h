//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "AuxKernel.h"

// Forward Declarations
class ValueAux;

template <>
InputParameters validParams<ValueAux>();

class ValueAux : public AuxKernel
{
public:
  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  ValueAux(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;
  const VariableValue & _v;
};
