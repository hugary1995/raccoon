//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ADComputeFunctionThermalExpansionEigenstrain.h"

registerMooseObject("raccoonApp", ADComputeFunctionThermalExpansionEigenstrain);

InputParameters
ADComputeFunctionThermalExpansionEigenstrain::validParams()
{
  InputParameters params = ADComputeThermalExpansionEigenstrainBase::validParams();
  params.addClassDescription("Computes eigenstrain due to thermal expansion "
                             "with a functional(material) coefficient");
  params.addRequiredParam<MaterialPropertyName>("thermal_expansion_coeff", "Material Thermal expansion coefficient");
  return params;
}

ADComputeFunctionThermalExpansionEigenstrain::ADComputeFunctionThermalExpansionEigenstrain(
    const InputParameters & parameters)
  : ADComputeThermalExpansionEigenstrainBase(parameters),
    _thermal_expansion_coeff(getADMaterialProperty<Real>("thermal_expansion_coeff"))
{
}

void
ADComputeFunctionThermalExpansionEigenstrain::computeThermalStrain(ADReal & thermal_strain)
{
  if (_use_old_temperature)
    thermal_strain =
        _thermal_expansion_coeff[_qp] * (_temperature_old[_qp] - _stress_free_temperature[_qp]);
  else
    thermal_strain = _thermal_expansion_coeff[_qp] * (_temperature[_qp] - _stress_free_temperature[_qp]);

  // std::cout << "thermal_strain" << raw_value(thermal_strain) << std::endl;
  // std::cout << "_temperature" << raw_value(_temperature[_qp]) << std::endl;
  // std::cout << "_stress_free_temperature" << raw_value(_stress_free_temperature[_qp]) << std::endl;
}
