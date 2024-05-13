//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "NucleationMicroForceBase.h"

/**
 * The class implements the external driving force to recover a Drucker-Prager
 * strength envelope developed by Kumar et al. in 2022. See Kumar, A., Ravi-Chandar, K. &
 * Lopez-Pamies, O. The revisited phase-field approach to brittle fracture: application to
 * indentation and notch problems. Int J Fract 237, 83–100 (2022).
 * https://doi.org/10.1007/s10704-022-00653-z.
 */
class KLRNucleationMicroForce : public NucleationMicroForceBase
{
public:
  static InputParameters validParams();

  KLRNucleationMicroForce(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// The critical tensile strength
  const ADMaterialProperty<Real> & _sigma_ts;

  /// The critical compressive strength
  const ADMaterialProperty<Real> & _sigma_cs;

  /// The materiel and model dependent parameter
  const ADMaterialProperty<Real> & _delta;

  /// Quantifying how far is the stress state from Drucker Prager
  ADMaterialProperty<Real> & _druck_prager_balance;
};
