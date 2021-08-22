//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"
#include "RankTwoTensorForward.h"

class Rectifier : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();

  Rectifier(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  MaterialProperty<RankTwoTensor> & _D;
  const VariableValue & _d;
  const VariableGradient & _grad_d;
  const Real _D_small;
  const Real _D_large;
};
