//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LDPlasticDefgradBase.h"

InputParameters
LDPlasticDefgradBase::validParams()
{
  InputParameters params = LDPlasticDeformationBase::validParams();
  return params;
}

LDPlasticDefgradBase::LDPlasticDefgradBase(const InputParameters & params)
  : LDPlasticDeformationBase(params),
    _Fm(getADMaterialPropertyByName<RankTwoTensor>(
        prependBaseName("mechanical_deformation_gradient"))),
    _Fp(declareADProperty<RankTwoTensor>(prependBaseName("plastic_deformation_gradient"))),
    _Fp_old(getMaterialPropertyOldByName<RankTwoTensor>(
        prependBaseName("plastic_deformation_gradient")))
{
}

void
LDPlasticDefgradBase::initQpStatefulProperties()
{
  LDPlasticDeformationBase::initQpStatefulProperties();
  _Fp[_qp].setToIdentity();
}
