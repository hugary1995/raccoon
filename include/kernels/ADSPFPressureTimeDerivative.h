//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADTimeDerivative.h"
#include "BaseNameInterface.h"

class ADSPFPressureTimeDerivative : public ADTimeDerivative, public BaseNameInterface
{
public:
  static InputParameters validParams();

  ADSPFPressureTimeDerivative(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  const ADMaterialProperty<Real> & _M;
};
