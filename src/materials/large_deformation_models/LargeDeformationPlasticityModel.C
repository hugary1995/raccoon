//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LargeDeformationPlasticityModel.h"
#include "LargeDeformationElasticityModel.h"
#include "PsicDegModel.h"
InputParameters
LargeDeformationPlasticityModel::validParams()
{
  InputParameters params = Material::validParams();
  params += ADSingleVariableReturnMappingSolution::validParams();
  params += BaseNameInterface::validParams();

  params.addRequiredParam<MaterialName>("hardening_model", "Name of the plastic hardening model");

  params.set<bool>("compute") = false;
  params.suppressParameter<bool>("compute");
  params.addParam<MaterialName>("psic_model", "Name of the psic model");
  params.addParam<bool>("recover", false, "recovering?");
  params.addParam<UserObjectName>("solution", "The SolutionUserObject to extract data from.");
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
    _heat(declareADProperty<Real>(prependBaseName("plastic_heat_generation"))),
    _recover(getParam<bool>("recover")),
    _solution_object_ptr(NULL)
{
  if (!isParamValid("solution") && _recover == true)
    MaterialBase::mooseError("Need solution object!");

  if (_recover == true)
    _solution_object_ptr = &getUserObject<SolutionUserObject>("solution");
}

void
LargeDeformationPlasticityModel::initialSetup()
{
  _hardening_model = dynamic_cast<PlasticHardeningModel *>(&getMaterial("hardening_model"));
  if (!_hardening_model)
    paramError("hardening_model",
               "Plastic hardening model " + _hardening_model->name() + " is not compatible with " +
                   name());
  _psic_model = isParamValid("psic_model")
                    ? dynamic_cast<PsicDegModel *>(&getMaterial("psic_model"))
                    : nullptr;
}

void
LargeDeformationPlasticityModel::setQp(unsigned int qp)
{
  _qp = qp;
  _hardening_model->setQp(qp);
  if (isParamValid("psic_model"))
  {
    _psic_model->setQp(qp);
  }
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

  _ep[_qp] = 0;
  _Fp[_qp].setToIdentity();
  //}

  std::vector<std::string> indices = {"x", "y", "z"};
  if (_recover)
  {
    _ep[_qp] =
        _solution_object_ptr->pointValue(_t,
                                         _current_elem->true_centroid(),
                                         "effective_plastic_strain_" + std::to_string(_qp + 1),
                                         nullptr);

    for (int i_ind = 0; i_ind < 3; i_ind++)
      for (int j_ind = 0; j_ind < 3; j_ind++)
      {
        _Fp[_qp](i_ind, j_ind) =
            _solution_object_ptr->pointValue(_t,
                                             _current_elem->true_centroid(),
                                             "plastic_deformation_gradie_" + indices[i_ind] +
                                                 indices[j_ind] + "_" + std::to_string(_qp + 1),
                                             nullptr);
      }
  }
}
