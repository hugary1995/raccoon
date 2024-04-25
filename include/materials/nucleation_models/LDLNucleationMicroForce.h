//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "NucleationMicroForceBase.h"

/**
 * The class implements the external driving force to recover a Drucker-Prager
 * strength envelope. See Larsen et. al. https://doi.org/10.48550/arXiv.2401.13938 for model 2024.
 */

class LDLNucleationMicroForce : public NucleationMicroForceBase
{
public:
  static InputParameters validParams();

  LDLNucleationMicroForce(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// The critical tensile strength
  const ADMaterialProperty<Real> & _sigma_ts;

  /// The critical hydrostatic strength
  const ADMaterialProperty<Real> & _sigma_hs;

  /// The materiel and model dependent parameter
  ADMaterialProperty<Real> & _delta;

  /// Whether to use h correction formula for delta
  bool _h_correction;
};
