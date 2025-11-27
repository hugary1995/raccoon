//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "CNHIsotropicElasticity.h"
#include "RaccoonUtils.h"

registerMooseObject("raccoonApp", CNHIsotropicElasticity);

InputParameters
CNHIsotropicElasticity::validParams()
{
  InputParameters params = LargeDeformationElasticityModel::validParams();
  params.addClassDescription("Isotropic Compressible Neo-Hookean hyperelasticity model.");

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

CNHIsotropicElasticity::CNHIsotropicElasticity(const InputParameters & parameters)
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
CNHIsotropicElasticity::computeMandelStress(const ADRankTwoTensor & Fe,
                                            const bool plasticity_update)
{
  ADRankTwoTensor stress;

  if (_decomposition == Decomposition::none)
    stress = computeMandelStressNoDecomposition(Fe, plasticity_update);
  else if (_decomposition == Decomposition::voldev)
    stress = computeMandelStressVolDevDecomposition(Fe, plasticity_update);
  else
    paramError("decomposition", "Unsupported decomposition type.");

  return stress;
}

ADRankTwoTensor
CNHIsotropicElasticity::computeMandelStressNoDecomposition(const ADRankTwoTensor & Fe,
                                                           const bool plasticity_update)
{
  using std::sqrt;
  // We use the left Cauchy-Green strain
  ADRankTwoTensor strain;
  if (plasticity_update)
  {
    ADRankTwoTensor expFe = RaccoonUtils::exp(Fe);
    strain = expFe * expFe.transpose();
  }
  else
    strain = Fe * Fe.transpose();

  ADReal J = sqrt(strain.det());

  const ADRankTwoTensor I2(ADRankTwoTensor::initIdentity);
  // Here, we keep the volumetric part no matter what. But ideally, in the case of J2 plasticity,
  // the volumetric part of the flow should be zero, and we could save some operations.
  ADRankTwoTensor stress_intact = 0.5 * _K[_qp] * (J * J - 1) * I2 + _G[_qp] * strain.deviatoric();
  ADRankTwoTensor stress = _g[_qp] * stress_intact;

  // If plasticity_update == false, then we are not in the middle of a plasticity update, hence we
  // compute the strain energy density
  if (!plasticity_update)
  {
    using std::log;
    using std::pow;
    ADRankTwoTensor strain_bar = pow(J, -2. / 3.) * strain;
    ADReal U = 0.5 * _K[_qp] * (0.5 * (J * J - 1) - log(J));
    ADReal W = 0.5 * _G[_qp] * (strain_bar.trace() - 3.0);
    _psie_active[_qp] = U + W;
    _psie[_qp] = _g[_qp] * _psie_active[_qp];
    _dpsie_dd[_qp] = _dg_dd[_qp] * _psie_active[_qp];
  }

  return stress;
}

ADRankTwoTensor
CNHIsotropicElasticity::computeMandelStressVolDevDecomposition(const ADRankTwoTensor & Fe,
                                                               const bool plasticity_update)
{
  using std::sqrt;
  // We use the left Cauchy-Green strain
  ADRankTwoTensor strain;
  if (plasticity_update)
  {
    ADRankTwoTensor expFe = RaccoonUtils::exp(Fe);
    strain = expFe * expFe.transpose();
  }
  else
    strain = Fe * Fe.transpose();

  ADReal J = sqrt(strain.det());

  const ADRankTwoTensor I2(ADRankTwoTensor::initIdentity);
  // Here, we keep the volumetric part no matter what. But ideally, in the case of J2 plasticity,
  // the volumetric part of the flow should be zero, and we could save some operations.
  ADRankTwoTensor stress_vol = 0.5 * _K[_qp] * (J * J - 1) * I2;
  ADRankTwoTensor stress_dev = _G[_qp] * strain.deviatoric();
  ADRankTwoTensor stress =
      J > 1 ? _g[_qp] * (stress_vol + stress_dev) : stress_vol + _g[_qp] * stress_dev;

  // If plasticity_update == false, then we are not in the middle of a plasticity update, hence we
  // compute the strain energy density
  if (!plasticity_update)
  {
    using std::log;
    using std::pow;
    ADRankTwoTensor strain_bar = pow(J, -2. / 3.) * strain;
    ADReal U = 0.5 * _K[_qp] * (0.5 * (J * J - 1) - log(J));
    ADReal W = 0.5 * _G[_qp] * (strain_bar.trace() - 3.0);
    _psie_active[_qp] = J > 1 ? U + W : W;
    _psie[_qp] = _g[_qp] * _psie_active[_qp];
    _dpsie_dd[_qp] = _dg_dd[_qp] * _psie_active[_qp];
  }

  return stress;
}
