//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SmallDeformationPlasticityModel.h"
#include "SmallDeformationElasticityModel.h"

InputParameters
SmallDeformationPlasticityModel::validParams()
{
  InputParameters params = Material::validParams();
  params += ADSingleVariableReturnMappingSolution::validParams();
  params.addParam<std::string>("base_name",
                               "Optional parameter that allows the user to define "
                               "multiple mechanics material systems on the same "
                               "block, i.e. for multiple phases");

  params.addRequiredParam<MaterialName>("hardening_model", "Name of the plastic hardening model");

  params.set<bool>("compute") = false;
  params.suppressParameter<bool>("compute");

  return params;
}

SmallDeformationPlasticityModel::SmallDeformationPlasticityModel(const InputParameters & parameters)
  : Material(parameters),
    ADSingleVariableReturnMappingSolution(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _plastic_strain(declareADProperty<RankTwoTensor>(_base_name + "plastic_strain")),
    _plastic_strain_old(getMaterialPropertyOldByName<RankTwoTensor>(_base_name + "plastic_strain")),
    _ep(declareADProperty<Real>(_base_name + "effective_plastic_strain")),
    _ep_old(getMaterialPropertyOldByName<Real>(_base_name + "effective_plastic_strain")),
    _Np(declareADProperty<RankTwoTensor>(_base_name + "flow_direction"))
{
}

void
SmallDeformationPlasticityModel::initialSetup()
{
  _hardening_model = dynamic_cast<PlasticHardeningModel *>(&getMaterial("hardening_model"));
  if (!_hardening_model)
    paramError("hardening_model",
               "Plastic hardening model " + _hardening_model->name() + " is not compatible with " +
                   name());
}

void
SmallDeformationPlasticityModel::setQp(unsigned int qp)
{
  _qp = qp;
  _hardening_model->setQp(qp);
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
  _ep[_qp] = 0;
}
