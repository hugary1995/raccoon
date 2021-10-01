//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"
#include "DerivativeMaterialPropertyNameInterface.h"

class ADPFFSource : public ADKernelValue, public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  ADPFFSource(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  /// Name of the Helmholtz free energy
  const MaterialPropertyName _psi_name;

  /// The derivative of Helmholtz free energy w.r.t. the phase field
  const ADMaterialProperty<Real> & _dpsi_dd;
};
