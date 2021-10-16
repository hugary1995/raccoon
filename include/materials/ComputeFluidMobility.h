//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "RankTwoTensorForward.h"
#include "BaseNameInterface.h"

class ComputeFluidMobility : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();

  ComputeFluidMobility(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  ADRankTwoTensor computeMatrixMob();
  ADRankTwoTensor computeFractureMob();

  /// fluid mobility tensor
  ADMaterialProperty<RankTwoTensor> & _fluid_mob;

  /// crack opneing
  const ADMaterialProperty<Real> & _wn;

  /// The phase field
  const ADVariableValue & _d;

  /// The gradient of the phase field
  const ADVariableGradient & _grad_d;

  /// fluid phase viscosity
  const ADMaterialProperty<Real> & _eta;

  /// intrinsic permeability of the porous domain
  const ADMaterialProperty<Real> & _K;

  /// damage exponent in crack fluid mobility
  const Real & _mob_eps;
};
