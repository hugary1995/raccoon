//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElementIntegralVariablePostprocessor.h"
#include "Function.h"

// Forward Declarations
class ElementIntegralVariableFunctionPostprocessor;

template <>
InputParameters validParams<ElementIntegralVariableFunctionPostprocessor>();

/**
 * This postprocessor computes a volume integral of the specified variable times the specified
 * function.
 */
class ElementIntegralVariableFunctionPostprocessor : public ElementIntegralVariablePostprocessor
{
public:
  ElementIntegralVariableFunctionPostprocessor(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral() override;

  /// function
  const Function & _function;
};
