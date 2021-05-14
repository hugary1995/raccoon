//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "HenckyIsotropicElasticity.h"
#include "RaccoonUtils.h"

registerMooseObject("raccoonApp", HenckyIsotropicElasticity);

InputParameters
HenckyIsotropicElasticity::validParams()
{
  InputParameters params = LargeDeformationElasticityModel::validParams();

  params.addRequiredParam<MaterialPropertyName>("bulk_modulus", "The bulk modulus $\\K$");
  params.addRequiredParam<MaterialPropertyName>("shear_modulus", "The shear modulus $\\G$");

  return params;
}

HenckyIsotropicElasticity::HenckyIsotropicElasticity(const InputParameters & parameters)
  : LargeDeformationElasticityModel(parameters),
    _K(getADMaterialProperty<Real>("bulk_modulus")),
    _G(getADMaterialProperty<Real>("shear_modulus"))
{
}

ADRankTwoTensor
HenckyIsotropicElasticity::computeMandelStress(const ADRankTwoTensor & Fe)
{
  const ADRankTwoTensor I2(ADRankTwoTensor::initIdentity);
  ADRankTwoTensor Ce = Fe.transpose() * Fe;
  ADRankTwoTensor ee = 0.5 * RaccoonUtils::log(Ce);
  ADRankTwoTensor stress = _K[_qp] * ee.trace() * I2 + 2 * _G[_qp] * ee.deviatoric();
  return stress;
}

ADRankTwoTensor
HenckyIsotropicElasticity::computeMandelStressExponentialUpdate(const ADRankTwoTensor & Fe)
{
  const ADRankTwoTensor I2(ADRankTwoTensor::initIdentity);
  // The following operations cancel out with an exponentiation of Fe:
  // ADRankTwoTensor Ce = Fe.transpose() * Fe;
  // ADRankTwoTensor ee = 0.5 * RaccoonUtils::log(Ce);
  ADRankTwoTensor stress = _K[_qp] * Fe.trace() * I2 + 2 * _G[_qp] * Fe.deviatoric();
  return stress;
}
