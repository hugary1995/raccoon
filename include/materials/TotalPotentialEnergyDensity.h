//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"

/**
 * Computes the total potential energy density
 *
 *   psi_total = psie                         (stored, degraded elastic energy  g*psie+ + psie-)
 *             + Gc/c0 * (alpha/l + l|grad d|^2)   (fracture surface energy  Gc*gamma)
 *             + (penalty/2) <d - d_old>_-^2        (penalty irreversibility energy)
 *
 * as a plain (non-AD) Real material property. Its domain integral is the total potential energy
 * Psi whose gradient is the assembled nonlinear residual, which makes it a consistent merit for a
 * custom outer (nonlinear-preconditioner) line search. No AD is needed because only the value is
 * used (never differentiated).
 */
class TotalPotentialEnergyDensity : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();

  TotalPotentialEnergyDensity(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// Stored (degraded) elastic strain energy density, g*psie_active + psie_inactive
  const ADMaterialProperty<Real> & _psie;

  /// Crack geometric function alpha(d)
  const ADMaterialProperty<Real> & _alpha;

  /// Fracture toughness Gc
  const ADMaterialProperty<Real> & _Gc;

  /// Normalization constant c0
  const ADMaterialProperty<Real> & _c0;

  /// Phase-field regularization length l
  const ADMaterialProperty<Real> & _l;

  /// Phase-field value, gradient, and old value (for the penalty irreversibility energy)
  const VariableValue & _d;
  const VariableGradient & _grad_d;
  const VariableValue & _d_old;

  /// Penalty coefficient (gamma) for the irreversibility energy; 0 disables it
  const Real _penalty;

  /// Viscosity coefficient (eta) for the viscous incremental energy (eta/2/dt)(d-d_old)^2; 0 disables
  const Real _viscosity;

  /// The computed total potential energy density (plain Real; value only)
  MaterialProperty<Real> & _psi_total;
};
