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
  params.addClassDescription("Isotropic Hencky-type hyperelasticity model. The logarithmic right "
                             "Cauchy-Green strain tensor is used as the strain measure.");

  params.addRequiredParam<MaterialPropertyName>("bulk_modulus", "The bulk modulus $K$");
  params.addRequiredParam<MaterialPropertyName>("shear_modulus", "The shear modulus $G$");

  params.addRequiredCoupledVar("phase_field", "Name of the phase-field (damage) variable");
  params.addParam<MaterialPropertyName>(
      "strain_energy_density",
      "psie",
      "Name of the strain energy density computed by this material model");
  params.addParam<MaterialPropertyName>("degradation_function", "g", "The degradation function");
  params.addParam<MooseEnum>(
      "decomposition", MooseEnum("NONE SPECTRAL VOLDEV", "NONE"), "The decomposition method");

  return params;
}

HenckyIsotropicElasticity::HenckyIsotropicElasticity(const InputParameters & parameters)
  : LargeDeformationElasticityModel(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _K(getADMaterialPropertyByName<Real>(prependBaseName("bulk_modulus", true))),
    _G(getADMaterialPropertyByName<Real>(prependBaseName("shear_modulus", true))),

    _d_name(getVar("phase_field", 0)->name()),

    // The strain energy density and its derivatives
    _psie_name(prependBaseName("strain_energy_density", true)),
    _psie(declareADProperty<Real>(_psie_name)),
    _psie_active(declareADProperty<Real>(_psie_name + "_active")),
    _dpsie_dd(declareADProperty<Real>(derivativePropertyName(_psie_name, {_d_name}))),

    // The degradation function and its derivatives
    _g_name(prependBaseName("degradation_function", true)),
    _g(getADMaterialProperty<Real>(_g_name)),
    _dg_dd(getADMaterialProperty<Real>(derivativePropertyName(_g_name, {_d_name}))),

    _decomposition(getParam<MooseEnum>("decomposition").getEnum<Decomposition>())
{
}

ADRankTwoTensor
HenckyIsotropicElasticity::computeMandelStress(const ADRankTwoTensor & Fe,
                                               const bool plasticity_update)
{
  ADRankTwoTensor stress;

  if (_decomposition == Decomposition::none)
    stress = computeMandelStressNoDecomposition(Fe, plasticity_update);
  else if (_decomposition == Decomposition::voldev)
    stress = computeMandelStressVolDevDecomposition(Fe, plasticity_update);
  else if (_decomposition == Decomposition::spectral)
    stress = computeMandelStressSpectralDecomposition(Fe, plasticity_update);
  else
    paramError("decomposition", "Unsupported decomposition type.");

  return stress;
}

ADRankTwoTensor
HenckyIsotropicElasticity::computeMandelStressNoDecomposition(const ADRankTwoTensor & Fe,
                                                              const bool plasticity_update)
{
  ADRankTwoTensor strain = Fe;
  // If this is called during a plasticity update, we need to first exponentiate Fe, where Fe should
  // be some plastic flow. The foolowing operations cancel out with an exponentiation of Fe, so we
  // only do this in the case of exponentiate == false
  if (!plasticity_update)
    strain = 0.5 * RaccoonUtils::log(Fe.transpose() * Fe);

  const ADRankTwoTensor I2(ADRankTwoTensor::initIdentity);
  // Here, we keep the volumetric part no matter what. But ideally, in the case of J2 plasticity,
  // the volumetric part of the flow should be zero, and we could save some operations.
  ADRankTwoTensor stress_intact = _K[_qp] * strain.trace() * I2 + 2 * _G[_qp] * strain.deviatoric();
  ADRankTwoTensor stress = _g[_qp] * stress_intact;

  // If plasticity_update == false, then we are not in the middle of a plasticity update, hence we
  // compute the strain energy density
  if (!plasticity_update)
  {
    // It is convenient that the Mandel stress is conjugate to the log strain
    _psie_active[_qp] = 0.5 * stress_intact.doubleContraction(strain);
    _psie[_qp] = _g[_qp] * _psie_active[_qp];
    _dpsie_dd[_qp] = _dg_dd[_qp] * _psie_active[_qp];
  }

  return stress;
}

ADRankTwoTensor
HenckyIsotropicElasticity::computeMandelStressVolDevDecomposition(const ADRankTwoTensor & Fe,
                                                                  const bool plasticity_update)
{
  ADRankTwoTensor strain = Fe;
  // If this is called during a plasticity update, we need to first exponentiate Fe, where Fe should
  // be some plastic flow. The foolowing operations cancel out with an exponentiation of Fe, so we
  // only do this in the case of exponentiate == false
  if (!plasticity_update)
    strain = 0.5 * RaccoonUtils::log(Fe.transpose() * Fe);

  const ADRankTwoTensor I2(ADRankTwoTensor::initIdentity);
  // Here, we keep the volumetric part no matter what. But ideally, in the case of J2 plasticity,
  // the volumetric part of the flow should be zero, and we could save some operations.
  ADReal strain_tr = strain.trace();
  ADReal strain_tr_pos = RaccoonUtils::Macaulay(strain_tr);
  ADReal strain_tr_neg = strain_tr - strain_tr_pos;
  ADRankTwoTensor strain_dev = strain.deviatoric();

  ADRankTwoTensor stress_intact = _K[_qp] * strain_tr * I2 + 2 * _G[_qp] * strain_dev;
  ADRankTwoTensor stress_neg = _K[_qp] * strain_tr_neg * I2;
  ADRankTwoTensor stress_pos = stress_intact - stress_neg;
  ADRankTwoTensor stress = _g[_qp] * stress_pos + stress_neg;

  // If plasticity_update == false, then we are not in the middle of a plasticity update, hence we
  // compute the strain energy density
  if (!plasticity_update)
  {
    ADReal psie_intact =
        0.5 * _K[_qp] * strain_tr * strain_tr + _G[_qp] * strain_dev.doubleContraction(strain_dev);
    ADReal psie_inactive = 0.5 * _K[_qp] * strain_tr_neg * strain_tr_neg;
    _psie_active[_qp] = psie_intact - psie_inactive;
    _psie[_qp] = _g[_qp] * _psie_active[_qp] + psie_inactive;
    _dpsie_dd[_qp] = _dg_dd[_qp] * _psie_active[_qp];
  }

  return stress;
}

ADRankTwoTensor
HenckyIsotropicElasticity::computeMandelStressSpectralDecomposition(const ADRankTwoTensor & Fe,
                                                                    const bool plasticity_update)
{
  ADRankTwoTensor strain = Fe;
  // If this is called during a plasticity update, we need to first exponentiate Fe, where Fe should
  // be some plastic flow. The foolowing operations cancel out with an exponentiation of Fe, so we
  // only do this in the case of exponentiate == false
  if (!plasticity_update)
    strain = 0.5 * RaccoonUtils::log(Fe.transpose() * Fe);

  const ADReal lambda = _K[_qp] - 2 * _G[_qp] / LIBMESH_DIM;
  const ADRankTwoTensor I2(ADRankTwoTensor::initIdentity);
  ADReal strain_tr = strain.trace();
  ADReal strain_tr_pos = RaccoonUtils::Macaulay(strain_tr);

  // Spectral decomposition
  ADRankTwoTensor strain_pos = RaccoonUtils::spectralDecomposition(strain);

  // Stress
  ADRankTwoTensor stress_intact = _K[_qp] * strain.trace() * I2 + 2 * _G[_qp] * strain.deviatoric();
  ADRankTwoTensor stress_pos = lambda * strain_tr_pos * I2 + 2 * _G[_qp] * strain_pos;
  ADRankTwoTensor stress_neg = stress_intact - stress_pos;
  ADRankTwoTensor stress = _g[_qp] * stress_pos + stress_neg;
  // If plasticity_update == false, then we are not in the middle of a plasticity update, hence we
  // compute the strain energy density
  if (!plasticity_update)
  {
    ADReal psie_intact =
        0.5 * lambda * strain_tr * strain_tr + _G[_qp] * strain.doubleContraction(strain);
    _psie_active[_qp] = 0.5 * lambda * strain_tr_pos * strain_tr_pos +
                        _G[_qp] * strain_pos.doubleContraction(strain_pos);
    ADReal psie_inactive = psie_intact - _psie_active[_qp];
    _psie[_qp] = _g[_qp] * _psie_active[_qp] + psie_inactive;
    _dpsie_dd[_qp] = _dg_dd[_qp] * _psie_active[_qp];
  }
  return stress;
}
