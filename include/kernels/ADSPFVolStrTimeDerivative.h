//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADTimeKernel.h"
#include "BaseNameInterface.h"

class ADSPFVolStrTimeDerivative : public ADTimeKernel, public BaseNameInterface
{
public:
  static InputParameters validParams();

  ADSPFVolStrTimeDerivative(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  // @{ The current and the old strain
  const ADMaterialProperty<RankTwoTensor> & _strain;
  const MaterialProperty<RankTwoTensor> & _strain_old;
  // @}

  /// The Biot coefficient
  const ADMaterialProperty<Real> & _alpha;
};
