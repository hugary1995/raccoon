//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SmallDeformationPlasticityModel.h"
#include "SmallDeformationElasticityModel.h"

InputParameters
SmallDeformationPlasticityModel::validParams()
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

SmallDeformationPlasticityModel::SmallDeformationPlasticityModel(const InputParameters & parameters)
  : Material(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _plastic_strain(declareADProperty<RankTwoTensor>(_base_name + "plastic_strain")),
    _effective_plastic_strain(declareADProperty<Real>(_base_name + "effective_plastic_strain"))
{
}

void
SmallDeformationPlasticityModel::setElasticityModel(
    SmallDeformationElasticityModel * elasticity_model)
{
  _elasticity_model = elasticity_model;
}

void
SmallDeformationPlasticityModel::initQpStatefulProperties()
{
  _plastic_strain[_qp].zero();
  _effective_plastic_strain[_qp] = 0;
}
