//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once
#include "Material.h"
#include "ComputeThermalExpansionEigenstrainBase.h"

/**
 * ADComputeFunctionThermalExpansionEigenstrain computes an eigenstrain for thermal expansion
 * with a functional(material) expansion coefficient.
 */
class ADComputeFunctionThermalExpansionEigenstrain : public ADComputeThermalExpansionEigenstrainBase
{
public:
  static InputParameters validParams();

  ADComputeFunctionThermalExpansionEigenstrain(const InputParameters & parameters);

protected:
  // virtual void computeThermalStrain(ADReal & thermal_strain) override;
  virtual ADReal computeThermalStrain() override;
  const ADMaterialProperty<Real> & _thermal_expansion_coeff;
};
