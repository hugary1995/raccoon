//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernel.h"

class ADPoroMechanicsCoupling : public ADKernel
{
public:
  static InputParameters validParams();

  ADPoroMechanicsCoupling(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual();

  /// biot coefficient
  const ADMaterialProperty<Real> & _coefficient;

  /// fluid pore pressure
  const ADVariableValue & _porepressure;

  /// component integer
  unsigned int _component;
};
