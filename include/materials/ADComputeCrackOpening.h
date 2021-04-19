//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "RankTwoTensorForward.h"

class ADComputeCrackOpening : public Material
{
public:
  static InputParameters validParams();

  ADComputeCrackOpening(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// crack opneing 
  ADMaterialProperty<Real> & _wn;

  /// damage and gradient of damage
  const ADVariableValue & _d;
  const ADVariableGradient & _grad_d;

  /// strain tensor at current and previous time step
  const ADMaterialProperty<RankTwoTensor> & _strain;
  const MaterialProperty<RankTwoTensor> & _strain_old;
};
