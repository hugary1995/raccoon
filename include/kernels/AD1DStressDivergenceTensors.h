//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADStressDivergenceTensors.h"

class AD1DStressDivergenceTensors : public ADStressDivergenceTensors
{
public:
  static InputParameters validParams();

  AD1DStressDivergenceTensors(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  const MaterialProperty<Real> & _A;
};
