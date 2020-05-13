//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelValue.h"
#include "DerivativeMaterialPropertyNameInterface.h"

class ADPFFBarrier : public ADKernelValue,
                                           public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  ADPFFBarrier(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  const ADMaterialProperty<Real> & _M;

  const ADMaterialProperty<Real> & _dw_dd;
};
