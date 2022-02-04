//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ElasticEnergyDensityBase.h"

InputParameters
ElasticEnergyDensityBase::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addParam<MaterialPropertyName>(
      "elastic_energy_density", "psie", "Name of the elastic energy density");
  return params;
}

ElasticEnergyDensityBase::ElasticEnergyDensityBase(const InputParameters & params)
  : DerivativeMaterialInterface<Material>(params),
    BaseNameInterface(params),
    _psie_name(prependBaseName("elastic_energy_density", true)),
    _psie(declareADProperty<Real>(_psie_name))
{
}

void
ElasticEnergyDensityBase::computeQpProperties()
{
  precomputeQpElasticEnergyDensity();
  computeQpElasticEnergyDensity();
  finalizeQpElasticEnergyDensity();
}
