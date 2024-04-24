//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "NucleationMicroForceBase.h"

/**
 * The class implements the external driving force to recover a Drucker-Prager
 * strength envelope. See Kumar et. al. https://doi.org/10.1016/j.jmps.2020.104027 for model 2020.
 */
class KLBFNucleationMicroForce : public NucleationMicroForceBase
{
public:
  static InputParameters validParams();

  KLBFNucleationMicroForce(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// The critical tensile strength
  const ADMaterialProperty<Real> & _sigma_ts;

  /// The critical compressive strength
  const ADMaterialProperty<Real> & _sigma_cs;

  /// The materiel and model dependent parameter
  const ADMaterialProperty<Real> & _delta;
};
