//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ADDarcyTimeDerivative.h"

registerMooseObject("raccoonApp", ADDarcyTimeDerivative);

InputParameters
ADDarcyTimeDerivative::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addClassDescription("The time derivative operator with the weak form of $(\\psi_i, "
                             "\\frac{\\partial u_h}{\\partial t})$.");
  params.addParam<MaterialPropertyName>("porosity_name", "porosity", "material property that computes the porosity");
  params.addParam<MaterialPropertyName>("density_name", "density", "material property that computes the density");
  return params;
}

ADDarcyTimeDerivative::ADDarcyTimeDerivative(const InputParameters & parameters)
  : ADKernelValue(parameters),
  _porosity(getADMaterialProperty<Real>("porosity_name")),
  _porosity_old(getMaterialPropertyOldByName<Real>(
                           getParam<MaterialPropertyName>("porosity_name"))),
  _density(getADMaterialProperty<Real>("density_name")),
  _density_old(getMaterialPropertyOldByName<Real>(
                           getParam<MaterialPropertyName>("density_name")))
{
}

ADReal
ADDarcyTimeDerivative::precomputeQpResidual()
{
  return (_porosity[_qp]*_density[_qp] - _porosity_old[_qp]*_density_old[_qp])/_dt;
}
