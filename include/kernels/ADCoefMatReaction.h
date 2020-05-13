//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"

class ADCoefMatReaction : public ADKernelValue
{
public:
  static InputParameters validParams();

  ADCoefMatReaction(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  const Real _coef;

  const std::vector<MaterialPropertyName> & _prop_names;

  const unsigned int _num_props;

  std::vector<const ADMaterialProperty<Real> *> _props;
};
