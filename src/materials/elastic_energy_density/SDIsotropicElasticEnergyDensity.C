//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SDIsotropicElasticEnergyDensity.h"

registerMooseObject("raccoonApp", SDIsotropicElasticEnergyDensity);

InputParameters
SDIsotropicElasticEnergyDensity::validParams()
{
  InputParameters params = SDElasticEnergyDensityBase::validParams();
  params.addClassDescription("Isotropic elasticity under small strain asumptions.");
  params.addRequiredParam<MaterialPropertyName>("bulk_modulus", "The bulk modulus $K$");
  params.addRequiredParam<MaterialPropertyName>("shear_modulus", "The shear modulus $G$");
  return params;
}

SDIsotropicElasticEnergyDensity::SDIsotropicElasticEnergyDensity(const InputParameters & parameters)
  : SDElasticEnergyDensityBase(parameters),
    _K(getADMaterialPropertyByName<Real>(prependBaseName("bulk_modulus", true))),
    _G(getADMaterialPropertyByName<Real>(prependBaseName("shear_modulus", true)))
{
}

ADRankTwoTensor
SDIsotropicElasticEnergyDensity::MandelStress(const ADRankTwoTensor & strain)
{
  const ADRankTwoTensor I2(ADRankTwoTensor::initIdentity);
  ADRankTwoTensor stress = _K[_qp] * strain.trace() * I2 + 2 * _G[_qp] * strain.deviatoric();
  return stress;
}

void
SDIsotropicElasticEnergyDensity::computeQpElasticEnergyDensity()
{
  ADReal strain_tr = _elastic_strain[_qp].trace();
  ADRankTwoTensor strain_dev = _elastic_strain[_qp].deviatoric();
  _psie[_qp] =
      0.5 * _K[_qp] * strain_tr * strain_tr + _G[_qp] * strain_dev.doubleContraction(strain_dev);
}

void
SDIsotropicElasticEnergyDensity::finalizeQpElasticEnergyDensity()
{
  _dpsie_dmechanical_strain[_qp] = MandelStress(_elastic_strain[_qp]);
  if (_plastic_strain)
    (*_dpsie_dplastic_strain)[_qp] = -MandelStress(_elastic_strain[_qp]);
}
