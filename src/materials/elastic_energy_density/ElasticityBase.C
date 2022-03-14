//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ElasticityBase.h"

InputParameters
ElasticityBase::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addParam<MaterialPropertyName>(
      "elastic_energy_density", "psie", "Name of the elastic energy density");
  return params;
}

ElasticityBase::ElasticityBase(const InputParameters & params)
  : DerivativeMaterialInterface<Material>(params),
    BaseNameInterface(params),
    _psie_name(prependBaseName("elastic_energy_density", true)),
    _psie(declareADProperty<Real>(_psie_name)),
    _mechanical_strain(getADMaterialProperty<RankTwoTensor>(prependBaseName("mechanical_strain"))),
    _dpsie_dmechanical_strain(declarePropertyDerivative<RankTwoTensor, true>(
        _psie_name, {prependBaseName("mechanical_strain")}))
{
}

void
ElasticityBase::computeQpProperties()
{
  precomputeQpElasticEnergyDensity();
  computeQpElasticEnergyDensity();
  finalizeQpElasticEnergyDensity();
}
