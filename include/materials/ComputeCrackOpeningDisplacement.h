//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "RankTwoTensorForward.h"
#include "BaseNameInterface.h"

class ComputeCrackOpeningDisplacement : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();

  ComputeCrackOpeningDisplacement(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// The approximated crack opening displacement
  ADMaterialProperty<Real> & _wn;

  /// The gradient of phase field
  const ADVariableGradient & _grad_d;

  /// The initial crack opening displacement
  const Real _w0;

  // @{ The current and the old strain
  const ADMaterialProperty<RankTwoTensor> & _strain;
  const MaterialProperty<RankTwoTensor> & _strain_old;
  // @}
};
