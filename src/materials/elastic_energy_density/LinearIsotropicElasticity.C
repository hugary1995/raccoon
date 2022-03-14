//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LinearIsotropicElasticity.h"

registerMooseObject("raccoonApp", LinearIsotropicElasticity);

InputParameters
LinearIsotropicElasticity::validParams()
{
  InputParameters params = ElasticityBase::validParams();
  params.addClassDescription("Isotropic elasticity under small strain asumptions.");
  params.addRequiredParam<MaterialPropertyName>("bulk_modulus", "The bulk modulus $K$");
  params.addRequiredParam<MaterialPropertyName>("shear_modulus", "The shear modulus $G$");
  return params;
}

LinearIsotropicElasticity::LinearIsotropicElasticity(const InputParameters & parameters)
  : ElasticityBase(parameters),
    _K(getADMaterialPropertyByName<Real>(prependBaseName("bulk_modulus", true))),
    _G(getADMaterialPropertyByName<Real>(prependBaseName("shear_modulus", true)))
{
}

void
LinearIsotropicElasticity::computeQpElasticEnergyDensity()
{
  ADReal strain_tr = _mechanical_strain[_qp].trace();
  ADRankTwoTensor strain_dev = _mechanical_strain[_qp].deviatoric();
  _psie[_qp] =
      0.5 * _K[_qp] * strain_tr * strain_tr + _G[_qp] * strain_dev.doubleContraction(strain_dev);
}

ADRankTwoTensor
LinearIsotropicElasticity::stress()
{
  const ADRankTwoTensor I2(ADRankTwoTensor::initIdentity);
  ADRankTwoTensor stress = _K[_qp] * _mechanical_strain[_qp].trace() * I2 +
                           2 * _G[_qp] * _mechanical_strain[_qp].deviatoric();
  return stress;
}
