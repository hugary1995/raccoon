//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ElastoplasticityBase.h"

InputParameters
ElastoplasticityBase::validParams()
{
  InputParameters params = ElasticityBase::validParams();
  return params;
}

ElastoplasticityBase::ElastoplasticityBase(const InputParameters & params)
  : ElasticityBase(params),
    _elastic_strain(getADMaterialProperty<RankTwoTensor>(prependBaseName("elastic_strain"))),
    _plastic_strain(getADMaterialProperty<RankTwoTensor>(prependBaseName("plastic_strain")))
{
}
