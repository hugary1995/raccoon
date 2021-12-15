//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"

class HydrostaticStrengthSurface : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();

  HydrostaticStrengthSurface(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// The strength
  const Real & _y;

  /// The stress tensor
  const ADMaterialProperty<RankTwoTensor> & _stress;

  ADMaterialProperty<Real> & _surface;
};
