//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SmallDeformationIsotropicElasticity.h"
#include "RaccoonUtils.h"

registerMooseObject("raccoonApp", SmallDeformationIsotropicElasticity);

InputParameters
SmallDeformationIsotropicElasticity::validParams()
{
  InputParameters params = SmallDeformationElasticityModel::validParams();

  params.addRequiredParam<MaterialPropertyName>("bulk_modulus", "The bulk modulus $\\K$");
  params.addRequiredParam<MaterialPropertyName>("shear_modulus", "The shear modulus $\\G$");

  params.addRequiredCoupledVar("phase_field", "Name of the phase-field (damage) variable");
  params.addParam<MaterialPropertyName>(
      "strain_energy_density",
      "we",
      "Name of the strain energy density computed by this material model");
  params.addParam<MaterialPropertyName>("degradation_function", "g", "The degradation function");
  params.addParam<MooseEnum>(
      "decomposition", MooseEnum("NONE SPECTRAL VOLDEV", "NONE"), "The decomposition method");

  return params;
}

SmallDeformationIsotropicElasticity::SmallDeformationIsotropicElasticity(
    const InputParameters & parameters)
  : SmallDeformationElasticityModel(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _K(getADMaterialProperty<Real>("bulk_modulus")),
    _G(getADMaterialProperty<Real>("shear_modulus")),

    _d_name(getVar("phase_field", 0)->name()),

    // The strain energy density and its derivatives
    _we_name(_base_name + getParam<MaterialPropertyName>("strain_energy_density")),
    _we(declareADProperty<Real>(_we_name)),
    _we_active(declareADProperty<Real>(_we_name + "_active")),
    _dwe_dd(declareADProperty<Real>(derivativePropertyName(_we_name, {_d_name}))),

    // The degradation function and its derivatives
    _g_name(_base_name + getParam<MaterialPropertyName>("degradation_function")),
    _g(getADMaterialProperty<Real>(_g_name)),
    _dg_dd(getADMaterialProperty<Real>(derivativePropertyName(_g_name, {_d_name}))),

    _decomposition(getParam<MooseEnum>("decomposition").getEnum<Decomposition>())
{
}

ADRankTwoTensor
SmallDeformationIsotropicElasticity::computeStress(const ADRankTwoTensor & strain)
{
  ADRankTwoTensor stress;

  if (_decomposition == Decomposition::none)
    stress = computeStressNoDecomposition(strain);
  else if (_decomposition == Decomposition::spectral)
    stress = computeStressSpectralDecomposition(strain);
  else if (_decomposition == Decomposition::voldev)
    stress = computeStressVolDevDecomposition(strain);
  else
    paramError("decomposition", "Unsupported decomposition type.");

  return stress;
}

ADRankTwoTensor
SmallDeformationIsotropicElasticity::computeStressNoDecomposition(const ADRankTwoTensor & strain)
{
  const ADRankTwoTensor I2(ADRankTwoTensor::initIdentity);
  ADRankTwoTensor stress_intact = _K[_qp] * strain.trace() * I2 + 2 * _G[_qp] * strain.deviatoric();
  ADRankTwoTensor stress = _g[_qp] * stress_intact;

  _we_active[_qp] = 0.5 * stress_intact.doubleContraction(strain);
  _we[_qp] = _g[_qp] * _we_active[_qp];
  _dwe_dd[_qp] = _dg_dd[_qp] * _we_active[_qp];

  return stress;
}

ADRankTwoTensor
SmallDeformationIsotropicElasticity::computeStressSpectralDecomposition(
    const ADRankTwoTensor & strain)
{
  const ADReal lambda = _K[_qp] - 2 * _G[_qp] / LIBMESH_DIM;
  const ADRankTwoTensor I2(ADRankTwoTensor::initIdentity);
  ADReal strain_tr = strain.trace();
  ADReal strain_tr_pos = RaccoonUtils::Macaulay(strain_tr);

  // Spectral decomposition
  ADRankTwoTensor strain_pos = RaccoonUtils::spectralDecomposition(strain);

  // Stress
  ADRankTwoTensor stress_intact = SmallDeformationIsotropicElasticity::computeStress(strain);
  ADRankTwoTensor stress_pos = lambda * strain_tr_pos * I2 + 2 * _G[_qp] * strain_pos;
  ADRankTwoTensor stress_neg = stress_intact - stress_pos;
  ADRankTwoTensor stress = _g[_qp] * stress_pos + stress_neg;

  // Strain energy density
  ADReal we_intact =
      0.5 * lambda * strain_tr * strain_tr + _G[_qp] * strain.doubleContraction(strain);
  _we_active[_qp] = 0.5 * lambda * strain_tr_pos * strain_tr_pos +
                    _G[_qp] * strain_pos.doubleContraction(strain_pos);
  ADReal we_inactive = we_intact - _we_active[_qp];
  _we[_qp] = _g[_qp] * _we_active[_qp] + we_inactive;
  _dwe_dd[_qp] = _dg_dd[_qp] * _we_active[_qp];

  return stress;
}

ADRankTwoTensor
SmallDeformationIsotropicElasticity::computeStressVolDevDecomposition(
    const ADRankTwoTensor & strain)
{
  const ADRankTwoTensor I2(ADRankTwoTensor::initIdentity);

  // Volumetric-deviatoric decomposition
  ADReal strain_tr = strain.trace();
  ADReal strain_tr_pos = RaccoonUtils::Macaulay(strain_tr);
  ADReal strain_tr_neg = strain_tr - strain_tr_pos;
  ADRankTwoTensor strain_dev = strain.deviatoric();

  // Stress
  ADRankTwoTensor stress_intact = SmallDeformationIsotropicElasticity::computeStress(strain);
  ADRankTwoTensor stress_neg = _K[_qp] * strain_tr_neg * I2;
  ADRankTwoTensor stress_pos = stress_intact - stress_neg;
  ADRankTwoTensor stress = _g[_qp] * stress_pos + stress_neg;

  // Strain energy density
  ADReal we_intact =
      0.5 * _K[_qp] * strain_tr * strain_tr + _G[_qp] * strain_dev.doubleContraction(strain_dev);
  ADReal we_inactive = 0.5 * _K[_qp] * strain_tr_neg * strain_tr_neg;
  _we_active[_qp] = we_intact - we_inactive;
  _we[_qp] = _g[_qp] * _we_active[_qp] + we_inactive;
  _dwe_dd[_qp] = _dg_dd[_qp] * _we_active[_qp];

  return stress;
}
