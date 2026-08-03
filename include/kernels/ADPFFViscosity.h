//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"

/**
 * Viscous (rate-dependent) regularization of the phase-field evolution. The weak form is
 * \f$ (w, \eta \dot{d}) \f$, adding a viscous term \f$ \eta \dot{d} \f$ to the phase-field
 * residual so the evolution reads \f$ \eta \dot{d} + \delta \Psi / \delta d = 0 \f$. This
 * smooths the otherwise abrupt (rate-independent) softening at crack nucleation/propagation;
 * the rate-independent model is recovered as \f$ \eta \to 0 \f$. Because \f$ \eta \dot{d} \f$
 * depends only on \f$ d \f$, it contributes solely to the phase-field diagonal Jacobian block.
 *
 * The rate is a backward-Euler difference \f$ \dot{d} \approx (d - d_{old})/\Delta t \f$ formed
 * directly from the previous converged solution (like ADPFFPenaltyIrreversibility), NOT via a
 * TimeKernel/TimeIntegrator -- the custom Executor system does not install a time integrator.
 */
class ADPFFViscosity : public ADKernelValue
{
public:
  static InputParameters validParams();

  ADPFFViscosity(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  /// Viscosity coefficient eta
  const Real _viscosity;

  /// Previous converged phase-field value d_old (constant w.r.t. the current Newton iterate)
  const VariableValue & _d_old;
};
