//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"

/**
 * Quadratic-penalty enforcement of phase-field irreversibility.
 *
 * Follows the penalized variational formulation of Gerasimov & De Lorenzis
 * (CMAME 2019) as used by Kopanicakova, Kothari & Krause (CMAME 2023). Adds the
 * term
 *
 *     (w, gamma * <d - d_old>_-)
 *
 * to the phase-field residual, where <x>_- = min(x, 0) and d_old is the previous
 * converged value of the phase field. This penalizes any decrease of the phase
 * field below d_old, replacing the variational-inequality (bound-constrained)
 * treatment with an unconstrained one -- so both the displacement and the
 * phase-field subproblems remain plain nonlinear solves, as required by the
 * field-split (ASPIN/MSPIN) nonlinear preconditioner.
 *
 * The penalty coefficient is
 *
 *     gamma = (Gc / l) * (1/tau_irr^2 - 1)
 *
 * and is supplied directly (e.g. computed with ${fparse ...} in the input).
 */
class ADPFFPenaltyIrreversibility : public ADKernelValue
{
public:
  static InputParameters validParams();

  ADPFFPenaltyIrreversibility(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  /// Penalty coefficient gamma = (Gc/l)(1/tau_irr^2 - 1)
  const Real _penalty;

  /// Old (previous converged) value of the phase field
  const VariableValue & _d_old;
};
