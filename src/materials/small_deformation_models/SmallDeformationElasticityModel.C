//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SmallDeformationElasticityModel.h"
#include "SmallDeformationPlasticityModel.h"

InputParameters
SmallDeformationElasticityModel::validParams()
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

SmallDeformationElasticityModel::SmallDeformationElasticityModel(const InputParameters & parameters)
  : Material(parameters),
    _plasticity_model(nullptr),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _elastic_strain(declareADProperty<RankTwoTensor>(_base_name + "elastic_strain"))
{
}

void
SmallDeformationElasticityModel::setQp(unsigned int qp)
{
  _qp = qp;
  if (_plasticity_model)
    _plasticity_model->setQp(qp);
}

void
SmallDeformationElasticityModel::setPlasticityModel(
    SmallDeformationPlasticityModel * plasticity_model)
{
  _plasticity_model = plasticity_model;
  _plasticity_model->setElasticityModel(this);
}

void
SmallDeformationElasticityModel::initQpStatefulProperties()
{
  _elastic_strain[_qp].zero();
}

void
SmallDeformationElasticityModel::updateState(const ADRankTwoTensor & mechanical_strain,
                                             ADRankTwoTensor & stress)
{
  _elastic_strain[_qp] = mechanical_strain;

  if (_plasticity_model)
    _plasticity_model->updateState(stress, _elastic_strain[_qp]);
  else
    stress = computeStress(_elastic_strain[_qp]);
}
