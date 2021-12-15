//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"

/**
 * The class implements the external driving force to recover a Drucker-Prager
 * strength envelope. See Kumar et. al. https://doi.org/10.1016/j.jmps.2020.104027.
 */
class DruckerPragerStrengthSurface : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();

  DruckerPragerStrengthSurface(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// The critical tensile strength
  const Real & _sigma_ts;

  /// The critical compressive strength
  const Real & _sigma_cs;

  /// The stress tensor
  const ADMaterialProperty<RankTwoTensor> & _stress;

  ADMaterialProperty<Real> & _surface;
};
