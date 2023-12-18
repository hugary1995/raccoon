//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LargeDeformationPlasticityModel.h"
#include "LargeDeformationElasticityModel.h"

InputParameters
LargeDeformationPlasticityModel::validParams()
{
  InputParameters params = Material::validParams();
  params += ADSingleVariableReturnMappingSolution::validParams();
  params += BaseNameInterface::validParams();

  params.addRequiredParam<MaterialName>("hardening_model", "Name of the plastic hardening model");

  params.set<bool>("compute") = false;
  params.suppressParameter<bool>("compute");

  return params;
}

LargeDeformationPlasticityModel::LargeDeformationPlasticityModel(const InputParameters & parameters)
  : Material(parameters),
    ADSingleVariableReturnMappingSolution(parameters),
    BaseNameInterface(parameters),
    _Fp(declareADProperty<RankTwoTensor>(prependBaseName("plastic_deformation_gradient"))),
    _Fp_old(getMaterialPropertyOldByName<RankTwoTensor>(
        prependBaseName("plastic_deformation_gradient"))),
    _ep(declareADProperty<Real>(prependBaseName("effective_plastic_strain"))),
    _ep_old(getMaterialPropertyOldByName<Real>(prependBaseName("effective_plastic_strain"))),
    _Np(declareADProperty<RankTwoTensor>(prependBaseName("flow_direction"))),
    _heat(declareADProperty<Real>(prependBaseName("plastic_heat_generation")))
{
}

void
LargeDeformationPlasticityModel::initialSetup()
{
  _hardening_model = dynamic_cast<PlasticHardeningModel *>(&getMaterial("hardening_model"));
  if (!_hardening_model)
    paramError("hardening_model",
               "Plastic hardening model " + getParam<MaterialName>("hardening_model") +
                   " is not compatible with " + name());
}

void
LargeDeformationPlasticityModel::setQp(unsigned int qp)
{
  _qp = qp;
  _hardening_model->setQp(qp);
}

void
LargeDeformationPlasticityModel::setElasticityModel(
    LargeDeformationElasticityModel * elasticity_model)
{
  _elasticity_model = elasticity_model;
}

void
LargeDeformationPlasticityModel::initQpStatefulProperties()
{
  _Fp[_qp].setToIdentity();
  _ep[_qp] = 0;
}
