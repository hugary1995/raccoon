//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "AuxKernel.h"

// Forward Declarations
class FPIVariableValueAux;

template <>
InputParameters validParams<FPIVariableValueAux>();

/**
 * Self auxiliary value
 */
class FPIVariableValueAux : public AuxKernel
{
public:
  static InputParameters validParams();

  FPIVariableValueAux(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;

  /// coupeld variable to read values from
  const VariableValue & _v;
};
