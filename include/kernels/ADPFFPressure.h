//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernelGrad.h"

class ADPFFPressure : public ADKernelGrad
{
public:
  static InputParameters validParams();

  ADPFFPressure(const InputParameters & parameters);

protected:
  virtual ADRealVectorValue precomputeQpResidual() override;

  const ADMaterialPropertyUserObject & _p_uo;
  const unsigned int _ndisp;
  std::vector<const ADVariableValue *> _disp;
  const Real _xi;
};
