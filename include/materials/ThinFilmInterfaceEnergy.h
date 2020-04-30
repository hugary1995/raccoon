//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADMaterial.h"

class ThinFilmInterfaceEnergy : public ADMaterial
{
public:
  static InputParameters validParams();

  ThinFilmInterfaceEnergy(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  const MaterialPropertyName _E_int_name;

  ADMaterialProperty<Real> & _E_int;

  const Real _coef;

  const unsigned int _ndisp;

  std::vector<const ADVariableValue *> _disp;
};
