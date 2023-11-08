//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ComputeFluidMobility.h"

registerMooseObject("raccoonApp", ComputeFluidMobility);

InputParameters
ComputeFluidMobility::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription("Compute mobility in porous media including effect of fracture");
  params.addParam<MaterialPropertyName>(
      "fluid_mobility", "Mf", "Name of the fluid mobility material property");
  params.addParam<MaterialPropertyName>(
      "crack_opening_displacement", "wn", "Name of the crack opening displacement");
  params.addRequiredCoupledVar("phase_field", "The phase field variable");
  params.addParam<MaterialPropertyName>("fluid_viscosity", "eta", "The fluid viscosity");
  params.addParam<MaterialPropertyName>(
      "intrinsic_permeability", "P", "The intrinsic permeability");
  params.addRequiredParam<Real>(
      "mobility_exponent",
      "The exponent used in the mobility equation to account for the effect of fracture");
  return params;
}

ComputeFluidMobility::ComputeFluidMobility(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _fluid_mob(declareADProperty<RankTwoTensor>(prependBaseName("fluid_mobility", true))),
    _wn(getADMaterialPropertyByName<Real>(prependBaseName("crack_opening_displacement", true))),
    _d(adCoupledValue("phase_field")),
    _grad_d(adCoupledGradient("phase_field")),
    _eta(getADMaterialProperty<Real>(prependBaseName("fluid_viscosity", true))),
    _K(getADMaterialProperty<Real>(prependBaseName("intrinsic_permeability", true))),
    _mob_eps(getParam<Real>("mobility_exponent"))
{
}

void
ComputeFluidMobility::computeQpProperties()
{
  ADRankTwoTensor matrix_mob = computeMatrixMob();
  ADRankTwoTensor fracture_mob = computeFractureMob();

  _fluid_mob[_qp] = matrix_mob + std::pow(_d[_qp], _mob_eps) * fracture_mob;
}

ADRankTwoTensor
ComputeFluidMobility::computeMatrixMob()
{
  ADRankTwoTensor identity(ADRankTwoTensor::initIdentity);
  return _K[_qp] / _eta[_qp] * identity;
}

ADRankTwoTensor
ComputeFluidMobility::computeFractureMob()
{
  const Real eps = 1e-15;
  ADRankTwoTensor identity(ADRankTwoTensor::initIdentity);
  ADRealVectorValue n;
  if (_grad_d[_qp].norm() > eps)
    n = _grad_d[_qp] / _grad_d[_qp].norm();

  ADRankTwoTensor nn;
  nn.vectorOuterProduct(n, n);

  return _wn[_qp] * _wn[_qp] / (12 * _eta[_qp]) * (identity - nn);
}
