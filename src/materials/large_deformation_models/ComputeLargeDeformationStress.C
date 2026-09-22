//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ComputeLargeDeformationStress.h"
#include "LargeDeformationElasticityModel.h"
#include "LargeDeformationPlasticityModel.h"
#include "LargeDeformationViscoelasticityModel.h"

registerMooseObject("raccoonApp", ComputeLargeDeformationStress);

InputParameters
ComputeLargeDeformationStress::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription("Stress calculator given an elasticity model, a plasticity model and "
                             "a viscoelasticity model. Large deformation is assumed.");

  params.addRequiredParam<MaterialName>("elasticity_model",
                                        "Name of the elastic stress-strain constitutive model");
  params.addParam<MaterialName>("plasticity_model", "Name of the plasticity model");
  params.addParam<MaterialName>("viscoelasticity_model", "Name of the viscoelasticity model");

  params.suppressParameter<bool>("use_displaced_mesh");
  return params;
}

ComputeLargeDeformationStress::ComputeLargeDeformationStress(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _Fm(getADMaterialProperty<RankTwoTensor>(prependBaseName("mechanical_deformation_gradient"))),
    _Fm_old(isParamValid("viscoelasticity_model")
                ? &getMaterialPropertyOld<RankTwoTensor>(
                      prependBaseName("mechanical_deformation_gradient"))
                : nullptr),
    _stress(declareADProperty<RankTwoTensor>(prependBaseName("stress")))
{
  if (getParam<bool>("use_displaced_mesh"))
    mooseError("The stress calculator needs to run on the undisplaced mesh.");
}

void
ComputeLargeDeformationStress::initialSetup()
{
  _elasticity_model =
      dynamic_cast<LargeDeformationElasticityModel *>(&getMaterial("elasticity_model"));
  if (!_elasticity_model)
    paramError("elasticity_model",
               "Elasticity model " + getParam<MaterialName>("elasticity_model") +
                   " is not compatible with ComputeLargeDeformationStress");

  _plasticity_model =
      isParamValid("plasticity_model")
          ? dynamic_cast<LargeDeformationPlasticityModel *>(&getMaterial("plasticity_model"))
          : nullptr;
  if (_plasticity_model)
    _elasticity_model->setPlasticityModel(_plasticity_model);

  _viscoelasticity_model = isParamValid("viscoelasticity_model")
                               ? dynamic_cast<LargeDeformationViscoelasticityModel *>(
                                     &getMaterial("viscoelasticity_model"))
                               : nullptr;

  // The elasticity, plasticity and viscoelasticity models are discrete materials manually driven
  // by this material, so MOOSE's automatic material dependency resolution cannot see the
  // properties they in turn depend on (e.g. the bulk/shear modulus or the degradation function).
  // Without propagating those dependencies here, boundary/face consumers of "stress" (such as a
  // SideIntegralPostprocessor) would reinit the discrete models without their inputs ever being
  // computed, silently yielding zero-valued properties.
  const auto & elasticity_deps = _elasticity_model->getMatPropDependencies();
  _material_property_dependencies.insert(elasticity_deps.begin(), elasticity_deps.end());
  if (_plasticity_model)
  {
    const auto & plasticity_deps = _plasticity_model->getMatPropDependencies();
    _material_property_dependencies.insert(plasticity_deps.begin(), plasticity_deps.end());
  }
  if (_viscoelasticity_model)
  {
    const auto & viscoelasticity_deps = _viscoelasticity_model->getMatPropDependencies();
    _material_property_dependencies.insert(viscoelasticity_deps.begin(),
                                           viscoelasticity_deps.end());
  }
}

void
ComputeLargeDeformationStress::initQpStatefulProperties()
{
  _stress[_qp].zero();
}

void
ComputeLargeDeformationStress::computeQpProperties()
{
  _elasticity_model->setQp(_qp);
  _elasticity_model->updateState(_Fm[_qp], _stress[_qp]);

  if (_viscoelasticity_model)
  {
    _viscoelasticity_model->setQp(_qp);
    _stress[_qp] += _viscoelasticity_model->computeCauchyStress(_Fm[_qp], (*_Fm_old)[_qp]);
  }
}
