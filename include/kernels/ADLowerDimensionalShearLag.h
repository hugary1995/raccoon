//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"
#include "BaseNameInterface.h"
#include "RankTwoTensorForward.h"

class ADLowerDimensionalShearLag : public ADKernelValue, public BaseNameInterface
{
public:
  static InputParameters validParams();

  ADLowerDimensionalShearLag(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  std::vector<const ADVariableValue *> _disps;

  const MaterialProperty<Real> & _s;

  const unsigned int _component;
};
