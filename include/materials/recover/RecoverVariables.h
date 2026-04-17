//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "Material.h"

class RecoverVariables : public Material
{
public:
  static InputParameters validParams();

  RecoverVariables(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;

  const std::vector<std::string> & _real_prop_names;
  const std::vector<Real> & _real_prop_values;

  unsigned int _num_props;

  std::vector<MaterialProperty<Real> *> _properties;

  std::vector<const ADMaterialProperty<Real> *> _get_properties;
};
