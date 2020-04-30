//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADMaterial.h"

class ADSumMaterial : public ADMaterial
{
public:
  static InputParameters validParams();

  ADSumMaterial(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;

  virtual void computeQpProperties() override;

  ADMaterialProperty<Real> & _sum;

  std::vector<MaterialPropertyName> _prop_names;

  unsigned int _num_props;

  std::vector<const ADMaterialProperty<Real> *> _props;
};
