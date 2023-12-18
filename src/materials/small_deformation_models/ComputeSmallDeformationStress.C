//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ComputeSmallDeformationStress.h"
#include "SmallDeformationElasticityModel.h"
#include "SmallDeformationPlasticityModel.h"

registerMooseObject("raccoonApp", ComputeSmallDeformationStress);

InputParameters
ComputeSmallDeformationStress::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription("The stress calculator given an elasticity model and a plasticity "
                             "model. Small deformation is assumed.");

  params.addRequiredParam<MaterialName>("elasticity_model",
                                        "Name of the elastic stress-strain constitutive model");
  params.addParam<MaterialName>("plasticity_model", "Name of the plasticity model");

  params.suppressParameter<bool>("use_displaced_mesh");
  return params;
}

ComputeSmallDeformationStress::ComputeSmallDeformationStress(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _mechanical_strain(getADMaterialProperty<RankTwoTensor>(prependBaseName("mechanical_strain"))),
    _stress(declareADProperty<RankTwoTensor>(prependBaseName("stress")))
{
  if (getParam<bool>("use_displaced_mesh"))
    mooseError("The stress calculator needs to run on the undisplaced mesh.");
}

void
ComputeSmallDeformationStress::initialSetup()
{
  _elasticity_model =
      dynamic_cast<SmallDeformationElasticityModel *>(&getMaterial("elasticity_model"));
  if (!_elasticity_model)
    paramError("elasticity_model",
               "Elasticity model " + getParam<MaterialName>("elasticity_model") +
                   " is not compatible with ComputeSmallDeformationStress");

  _plasticity_model =
      isParamValid("plasticity_model")
          ? dynamic_cast<SmallDeformationPlasticityModel *>(&getMaterial("plasticity_model"))
          : nullptr;
  if (_plasticity_model)
    _elasticity_model->setPlasticityModel(_plasticity_model);
}

void
ComputeSmallDeformationStress::initQpStatefulProperties()
{
  _stress[_qp].zero();
}

void
ComputeSmallDeformationStress::computeQpProperties()
{
  _elasticity_model->setQp(_qp);
  _elasticity_model->updateState(_mechanical_strain[_qp], _stress[_qp]);
}
