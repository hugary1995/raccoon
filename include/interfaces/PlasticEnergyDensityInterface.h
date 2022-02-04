//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

// MOOSE includes
#include "Material.h"
#include "DerivativeMaterialPropertyNameInterface.h"
#include "BaseNameInterface.h"

typedef std::tuple<ADReal, ADReal, ADReal> ElasticEnergyDensity;

class PlasticEnergyDensityInterface : public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  PlasticEnergyDensityInterface(Material * obj);

private:
  const VariableName _d_name;
  const MaterialPropertyName _psip_name;

protected:
  ADMaterialProperty<Real> & _psip;
  ADMaterialProperty<Real> & _psip_active;
  ADMaterialProperty<Real> & _dpsip_dd;
};

InputParameters
PlasticEnergyDensityInterface::validParams()
{
  InputParameters params = emptyInputParameters();
  params.addRequiredParam<VariableName>("phase_field", "The phase-field (damage) variable");
  params.addParam<MaterialPropertyName>(
      "plastic_energy_density",
      "psip",
      "Name of the plastic energy density computed by this material model");
  return params;
}

PlasticEnergyDensityInterface::PlasticEnergyDensityInterface(Material * obj)
  : DerivativeMaterialPropertyNameInterface(),
    _d_name(obj->parameters().get<VariableName>("phase_field")),
    _psip_name(dynamic_cast<const BaseNameInterface *>(obj)->prependBaseName(
        "plastic_energy_density", true)),
    _psip(obj->declareADProperty<Real>(_psip_name)),
    _psip_active(obj->declareADProperty<Real>(_psip_name + "_active")),
    _dpsip_dd(obj->declareADProperty<Real>(derivativePropertyName(_psip_name, {_d_name})))
{
}
