//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SDPlasticStrainBase.h"

InputParameters
SDPlasticStrainBase::validParams()
{
  InputParameters params = SDPlasticDeformationBase::validParams();
  return params;
}

SDPlasticStrainBase::SDPlasticStrainBase(const InputParameters & params)
  : SDPlasticDeformationBase(params),
    _mechanical_strain(
        getADMaterialPropertyByName<RankTwoTensor>(prependBaseName("mechanical_strain"))),
    _plastic_strain(declareADProperty<RankTwoTensor>(prependBaseName("plastic_strain"))),
    _plastic_strain_old(
        getMaterialPropertyOldByName<RankTwoTensor>(prependBaseName("plastic_strain")))
{
}

void
SDPlasticStrainBase::initQpStatefulProperties()
{
  SDPlasticDeformationBase::initQpStatefulProperties();
  _plastic_strain[_qp].zero();
}
