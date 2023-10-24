//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElementIntegralPostprocessor.h"

class SolutionChangeNorm : public ElementIntegralPostprocessor
{
public:
  static InputParameters validParams();

  SolutionChangeNorm(const InputParameters & parameters);

  virtual Real getValue() const override;

protected:
  virtual Real computeQpIntegral() override;

  /// Vector of variable values
  const std::vector<const VariableValue *> _us;

  /// Vector of old variable values
  const std::vector<const VariableValue *> _us_old;
};
