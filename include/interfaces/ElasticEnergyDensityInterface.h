//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

// MOOSE includes
#include "Material.h"
#include "DerivativeMaterialPropertyNameInterface.h"
#include "BaseNameInterface.h"

typedef std::tuple<ADReal, ADReal, ADReal> ElasticEnergyDensity;

enum class Decomposition
{
  none,
  spectral,
  voldev
};

class ElasticEnergyDensityInterface : public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  ElasticEnergyDensityInterface(Material * obj);

private:
  const VariableName _d_name;
  const MaterialPropertyName _psie_name;

protected:
  ADMaterialProperty<Real> & _psie;
  ADMaterialProperty<Real> & _psie_active;
  ADMaterialProperty<Real> & _dpsie_dd;
};

InputParameters
ElasticEnergyDensityInterface::validParams()
{
  InputParameters params = emptyInputParameters();
  params.addRequiredParam<VariableName>("phase_field", "The phase-field (damage) variable");
  params.addParam<MaterialPropertyName>(
      "elastic_energy_density",
      "psie",
      "Name of the elastic energy density computed by this material model");
  return params;
}

ElasticEnergyDensityInterface::ElasticEnergyDensityInterface(Material * obj)
  : DerivativeMaterialPropertyNameInterface(),
    _d_name(obj->parameters().get<VariableName>("phase_field")),
    _psie_name(dynamic_cast<const BaseNameInterface *>(obj)->prependBaseName(
        "elastic_energy_density", true)),
    _psie(obj->declareADProperty<Real>(_psie_name)),
    _psie_active(obj->declareADProperty<Real>(_psie_name + "_active")),
    _dpsie_dd(obj->declareADProperty<Real>(derivativePropertyName(_psie_name, {_d_name})))
{
}
