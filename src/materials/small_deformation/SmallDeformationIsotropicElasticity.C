//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SmallDeformationIsotropicElasticity.h"

InputParameters
SmallDeformationIsotropicElasticity::validParams()
{
  InputParameters params = SmallDeformationElasticityModel::validParams();

  params.addRequiredParam<MaterialPropertyName>("bulk_modulus", "The bulk modulus $\\K$");
  params.addRequiredParam<MaterialPropertyName>("shear_modulus", "The shear modulus $\\G$");

  return params;
}

SmallDeformationIsotropicElasticity::SmallDeformationIsotropicElasticity(
    const InputParameters & parameters)
  : SmallDeformationElasticityModel(parameters),
    _K(getADMaterialProperty<Real>("bulk_modulus")),
    _G(getADMaterialProperty<Real>("shear_modulus"))
{
}

ADRankTwoTensor
SmallDeformationIsotropicElasticity::computeStress(const ADRankTwoTensor & strain)
{
  const ADRankTwoTensor I2(ADRankTwoTensor::initIdentity);
  ADRankTwoTensor stress = _K[_qp] * strain.trace() * I2 + 2 * _G[_qp] * strain.deviatoric();
  return stress;
}
