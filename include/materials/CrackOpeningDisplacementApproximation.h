//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "RankTwoTensorForward.h"
#include "BaseNameInterface.h"

class CrackOpeningDisplacementApproximation : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();

  CrackOpeningDisplacementApproximation(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// The approximated crack opneing displacement
  ADMaterialProperty<Real> & _wn;

  /// The gradient of phase field
  const ADVariableGradient & _grad_d;

  /// The old strain
  const MaterialProperty<RankTwoTensor> & _strain_old;
};
