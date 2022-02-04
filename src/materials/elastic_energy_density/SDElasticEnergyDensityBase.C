//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SDElasticEnergyDensityBase.h"

InputParameters
SDElasticEnergyDensityBase::validParams()
{
  InputParameters params = ElasticEnergyDensityBase::validParams();
  return params;
}

SDElasticEnergyDensityBase::SDElasticEnergyDensityBase(const InputParameters & params)
  : ElasticEnergyDensityBase(params),
    _mechanical_strain(getADMaterialProperty<RankTwoTensor>(prependBaseName("mechanical_strain"))),
    _plastic_strain(hasADMaterialProperty<RankTwoTensor>(prependBaseName("plastic_strain"))
                        ? &getADMaterialProperty<RankTwoTensor>(prependBaseName("plastic_strain"))
                        : nullptr),
    _elastic_strain(declareADProperty<RankTwoTensor>(prependBaseName("elastic_strain"))),
    _dpsie_dmechanical_strain(declarePropertyDerivative<RankTwoTensor, true>(
        _psie_name, {prependBaseName("mechanical_strain")})),
    _dpsie_dplastic_strain(_plastic_strain ? &declarePropertyDerivative<RankTwoTensor, true>(
                                                 _psie_name, {prependBaseName("plastic_strain")})
                                           : nullptr)
{
}

void
SDElasticEnergyDensityBase::precomputeQpElasticEnergyDensity()
{
  _elastic_strain[_qp] = _mechanical_strain[_qp];
  if (_plastic_strain)
    _elastic_strain[_qp] -= (*_plastic_strain)[_qp];
}
