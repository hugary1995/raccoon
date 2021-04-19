//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "RankTwoTensorForward.h"

class ADComputeFluidMobility : public Material
{
public:
  static InputParameters validParams();

  ADComputeFluidMobility(const InputParameters & parameters);

protected:
  ADRankTwoTensor computeMatrixMob();
  ADRankTwoTensor computeFractureMob();

  virtual void computeQpProperties() override;

  /// fluid mobility tensor
  ADMaterialProperty<RankTwoTensor> & _fluid_mob;
  
  /// crack opneing 
  const ADMaterialProperty<Real> & _wn;

  /// damage variable
  const ADVariableValue & _d;
  const ADVariableGradient & _grad_d;

  /// fluid phase viscosity
  const ADMaterialProperty<Real> & _eta;
  
  /// intrinsic permeability of the porous domain
  const ADMaterialProperty<Real> & _K;

  /// damage exponent in crack fluid mobility 
  const Real & _mob_eps;
  
  /// initial crack opening value 
  const Real & _wcr;
};
