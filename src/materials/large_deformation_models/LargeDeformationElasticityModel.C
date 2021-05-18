//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LargeDeformationElasticityModel.h"
#include "LargeDeformationPlasticityModel.h"

InputParameters
LargeDeformationElasticityModel::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();

  params.set<bool>("compute") = false;
  params.suppressParameter<bool>("compute");

  return params;
}

LargeDeformationElasticityModel::LargeDeformationElasticityModel(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _plasticity_model(nullptr),
    _Fe(declareADProperty<RankTwoTensor>(prependBaseName("elastic_deformation_gradient")))
{
}

void
LargeDeformationElasticityModel::setQp(unsigned int qp)
{
  _qp = qp;
  if (_plasticity_model)
    _plasticity_model->setQp(qp);
}

void
LargeDeformationElasticityModel::setPlasticityModel(
    LargeDeformationPlasticityModel * plasticity_model)
{
  _plasticity_model = plasticity_model;
  _plasticity_model->setElasticityModel(this);
}

void
LargeDeformationElasticityModel::initQpStatefulProperties()
{
  _Fe[_qp].setToIdentity();
}

void
LargeDeformationElasticityModel::updateState(const ADRankTwoTensor & Fm, ADRankTwoTensor & stress)
{
  _Fe[_qp] = Fm;

  if (_plasticity_model)
    _plasticity_model->updateState(stress, _Fe[_qp]);
  else
    stress = computeCauchyStress(_Fe[_qp]);
}

ADRankTwoTensor
LargeDeformationElasticityModel::computeCauchyStress(const ADRankTwoTensor & Fe)
{
  return Fe.inverse().transpose() * computeMandelStress(Fe) * Fe.transpose() / Fe.det();
}
