//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "REFACTOR_CustomParsedFunctionBase.h"

/**
 * Class to evaluate the crack geometric function and automatically provide all derivatives.
 */
class REFACTOR_CrackGeometricFunction : public REFACTOR_CustomParsedFunctionBase
{
public:
  static InputParameters validParams();

  REFACTOR_CrackGeometricFunction(const InputParameters & parameters);

protected:
  void computeQpProperties() override;

private:
  ADReal computeNormalizationConstant();
  ADReal normalizationIntegrand(ADReal d);

  unsigned int _d_idx;

  ADMaterialProperty<Real> & _xi;
  ADMaterialProperty<Real> & _c0;

  Real _tolerance;
  unsigned int _max_its;

  ADReal _xi_0;
  ADReal _c0_0;
};
