//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LargeDeformationElasticityModel.h"
#include "LargeDeformationPlasticityModel.h"

InputParameters
LargeDeformationElasticityModel::validParams()
{
  InputParameters params = Material::validParams();
  params.addParam<std::string>("base_name",
                               "Optional parameter that allows the user to define "
                               "multiple mechanics material systems on the same "
                               "block, i.e. for multiple phases");

  params.set<bool>("compute") = false;
  params.suppressParameter<bool>("compute");

  return params;
}

LargeDeformationElasticityModel::LargeDeformationElasticityModel(const InputParameters & parameters)
  : Material(parameters),
    _plasticity_model(nullptr),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _Fe(declareADProperty<RankTwoTensor>(_base_name + "elastic_deformation_gradient"))
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
