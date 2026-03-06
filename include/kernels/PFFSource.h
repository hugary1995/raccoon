//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "KernelValue.h"
#include "DerivativeMaterialInterface.h"

class PFFSource : public DerivativeMaterialInterface<KernelValue>
{
public:
  static InputParameters validParams();

  PFFSource(const InputParameters & parameters);

protected:
  virtual Real precomputeQpResidual() override;

  virtual Real precomputeQpJacobian() override;

  /// Name of the Helmholtz free energy
  const MaterialPropertyName _psi_name;

  /// The derivative of Helmholtz free energy w.r.t. the phase field
  const MaterialProperty<Real> & _dpsi_dd;

  /// The 2nd derivative of Helmholtz free energy w.r.t. the phase field required for Jacobian
  const MaterialProperty<Real> & _d2psi_dd2;
};
