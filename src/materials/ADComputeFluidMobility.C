//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADComputeFluidMobility.h"

registerMooseObject("raccoonApp", ADComputeFluidMobility);

InputParameters
ADComputeFluidMobility::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("compute mobility in porous media including effect of damage");
  params.addParam<MaterialPropertyName>(
      "fluid_mobility_name", "fluid_mobility", "Name of fluid mobility material property");
  params.addRequiredCoupledVar("damage", "damage field");
  params.addParam<MaterialPropertyName>(
      "fluid_viscosity_name", "fluid_viscosity", "fluid_viscosity");
  params.addParam<MaterialPropertyName>(
      "intrinsic_permeability_name", "intrinsic_permeability", "intrinsic_permeability");
  params.addRequiredParam<Real>("mobility_exponent", "exponent used in mobility equation");
  params.addParam<Real>("initial_crack_opening", 0.0, "initial opening of a crack");
  return params;
}

ADComputeFluidMobility::ADComputeFluidMobility(const InputParameters & parameters)
  : Material(parameters),
    _fluid_mob(
        declareADProperty<RankTwoTensor>(getParam<MaterialPropertyName>("fluid_mobility_name"))),
    _wn(getADMaterialPropertyByName<Real>("wn")),
    _d(adCoupledValue("damage")),
    _grad_d(adCoupledGradient("damage")),
    _eta(getADMaterialProperty<Real>("fluid_viscosity_name")),
    _K(getADMaterialProperty<Real>("intrinsic_permeability_name")),
    _mob_eps(getParam<Real>("mobility_exponent")),
    _wcr(getParam<Real>("initial_crack_opening"))
{
}

ADRankTwoTensor
ADComputeFluidMobility::computeMatrixMob()
{
  ADRankTwoTensor identity(ADRankTwoTensor::initIdentity);
  return _K[_qp] / _eta[_qp] * identity;
}

ADRankTwoTensor
ADComputeFluidMobility::computeFractureMob()
{
  const Real eps = 1e-15;
  ADRankTwoTensor identity(ADRankTwoTensor::initIdentity);
  ADRealVectorValue n;
  if (_grad_d[_qp].norm() > eps)
    n = _grad_d[_qp] / _grad_d[_qp].norm();
  ADRankTwoTensor nn;
  nn.vectorOuterProduct(n, n);
  return (_wn[_qp] + _wcr) * (_wn[_qp] + _wcr) / (12 * _eta[_qp]) * (identity - nn);
}

void
ADComputeFluidMobility::computeQpProperties()
{
  ADRankTwoTensor matrix_mob = computeMatrixMob();
  ADRankTwoTensor fracture_mob = computeFractureMob();

  _fluid_mob[_qp] = matrix_mob + std::pow(_d[_qp], _mob_eps) * fracture_mob;
}
