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

HenckyIsotropicElasticity::HenckyIsotropicElasticity(const InputParameters & parameters)
  : LargeDeformationElasticityModel(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _K(getADMaterialPropertyByName<Real>(_base_name +
                                         getParam<MaterialPropertyName>("bulk_modulus"))),
    _G(getADMaterialPropertyByName<Real>(_base_name +
                                         getParam<MaterialPropertyName>("shear_modulus"))),

    _d_name(getVar("phase_field", 0)->name()),

    // The strain energy density and its derivatives
    _psie_name(_base_name + getParam<MaterialPropertyName>("strain_energy_density")),
    _psie(declareADProperty<Real>(_psie_name)),
    _psie_active(declareADProperty<Real>(_psie_name + "_active")),
    _dpsie_dd(declareADProperty<Real>(derivativePropertyName(_psie_name, {_d_name}))),

    // The degradation function and its derivatives
    _g_name(_base_name + getParam<MaterialPropertyName>("degradation_function")),
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
