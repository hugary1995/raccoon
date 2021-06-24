//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"

class CrackOpeningDensity : public Material
{
public:
  static InputParameters validParams();

  CrackOpeningDensity(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  const unsigned int _ndisp;
  std::vector<const ADVariableValue *> _disp;
  const ADVariableGradient & _grad_d;
  ADMaterialProperty<Real> & _gap;
};
