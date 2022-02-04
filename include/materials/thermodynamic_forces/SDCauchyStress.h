//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ThermodynamicForce.h"
#include "ADRankTwoTensorForward.h"

class SDCauchyStress : public ThermodynamicForce
{
public:
  static InputParameters validParams();

  SDCauchyStress(const InputParameters & params);

protected:
  virtual void computeQpProperties() override;

  /// The equilibrium stress: d(psi)/d(mechanical_strain)
  std::vector<const ADMaterialProperty<RankTwoTensor> *> _dpsis_dmechanical_strain;

  /// The Cauchy stress
  ADMaterialProperty<RankTwoTensor> & _stress;
};
