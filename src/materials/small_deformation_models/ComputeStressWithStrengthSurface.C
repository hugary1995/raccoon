//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ComputeStressWithStrengthSurface.h"

registerMooseObject("raccoonApp", ComputeStressWithStrengthSurface);

InputParameters
ComputeStressWithStrengthSurface::validParams()
{
  InputParameters params = Material::validParams();
  params += ADSingleVariableReturnMappingSolution::validParams();
  params += BaseNameInterface::validParams();

  params.addClassDescription("Compute small deformation stress with a strength surface.");
  params.addRequiredParam<MaterialPropertyName>("bulk_modulus", "The bulk modulus $K$");
  params.addRequiredParam<MaterialPropertyName>("shear_modulus", "The shear modulus $G$");
  params.addParam<MaterialPropertyName>(
      "structure_energy_density",
      "psin",
      "Name of the structure energy density computed by this material model");
  params.addParam<MaterialPropertyName>(
      "strain_energy_density",
      "psie",
      "Name of the strain energy density computed by this material model");
  params.addRequiredCoupledVar("phase_field", "The phase field variable");
  params.addParam<MaterialPropertyName>("degradation_function", "g", "The degradation function");
  params.addRequiredParam<Real>("gamma_1", "gamma 1");
  params.addRequiredParam<Real>("gamma_0", "gamma 0");
  return params;
}

ComputeStressWithStrengthSurface::ComputeStressWithStrengthSurface(
    const InputParameters & parameters)
  : Material(parameters),
    ADSingleVariableReturnMappingSolution(parameters),
    BaseNameInterface(parameters),
    DerivativeMaterialPropertyNameInterface(),
    _K(getADMaterialPropertyByName<Real>(prependBaseName("bulk_modulus", true))),
    _G(getADMaterialPropertyByName<Real>(prependBaseName("shear_modulus", true))),

    _mechanical_strain(getADMaterialProperty<RankTwoTensor>(prependBaseName("mechanical_strain"))),
    _stress(declareADProperty<RankTwoTensor>(prependBaseName("stress"))),
    _elastic_strain(declareADProperty<RankTwoTensor>(prependBaseName("elastic_strain"))),

    _c(declareADProperty<Real>(prependBaseName("effective_structured_strain"))),
    _c_old(getMaterialPropertyOldByName<Real>(prependBaseName("effective_structured_strain"))),
    _structured_strain(declareADProperty<RankTwoTensor>(prependBaseName("structured_strain"))),
    _structured_strain_old(
        getMaterialPropertyOldByName<RankTwoTensor>(prependBaseName("structured_strain"))),
    _N(declareADProperty<RankTwoTensor>(prependBaseName("flow_direction"))),

    _d_name(getVar("phase_field", 0)->name()),
    _psin_name(prependBaseName("structure_energy_density", true)),
    _psin_active(declareADProperty<Real>(_psin_name + "_active")),
    _psie_name(prependBaseName("strain_energy_density", true)),
    _psie(declareADProperty<Real>(_psie_name)),
    _psie_active(declareADProperty<Real>(_psie_name + "_active")),
    _g_name(prependBaseName("degradation_function", true)),
    _g(getADMaterialProperty<Real>(_g_name)),
    _dg_dd(getADMaterialProperty<Real>(derivativePropertyName(_g_name, {_d_name}))),

    _gamma_1(getParam<Real>("gamma_1")),
    _gamma_0(getParam<Real>("gamma_0"))
{
}

void
ComputeStressWithStrengthSurface::initQpStatefulProperties()
{
  _c[_qp] = 0;
  _structured_strain[_qp].zero();
}

ADRankTwoTensor
ComputeStressWithStrengthSurface::computeStress(const ADRankTwoTensor & strain)
{
  const ADRankTwoTensor I2(ADRankTwoTensor::initIdentity);
  return _g[_qp] * (_K[_qp] * strain.trace() * I2 + 2 * _G[_qp] * strain.deviatoric());
}

ADReal
ComputeStressWithStrengthSurface::updateTrialState(const ADReal & delta_c)
{
  const ADRankTwoTensor I2(ADRankTwoTensor::initIdentity);

  // Trial strain
  ADRankTwoTensor trial_strain = _mechanical_strain[_qp] - _structured_strain_old[_qp];

  // Flow direction
  ADReal n2 = trial_strain.doubleContraction(trial_strain);
  if (MooseUtils::absoluteFuzzyEqual(n2, 0))
    n2.value() = libMesh::TOLERANCE * libMesh::TOLERANCE;
  ADReal n = std::sqrt(n2);
  _N[_qp] = 2 / (2 - delta_c) * (trial_strain.deviatoric() / n / std::sqrt(2) + _gamma_1 * I2);

  // Effective trial stress
  return computeStress(trial_strain).doubleContraction(_N[_qp]);
}

void
ComputeStressWithStrengthSurface::computeQpProperties()
{
  ADReal delta_c = 0;
  ADReal effective_trial_stress = updateTrialState(delta_c);

  // Return mapping
  ADReal phi = computeResidual(effective_trial_stress, delta_c);
  if (phi > 0)
    returnMappingSolve(effective_trial_stress, delta_c, _console);

  // Update internal variables
  _c[_qp] = _c_old[_qp] + delta_c;
  _structured_strain[_qp] = _structured_strain_old[_qp] + delta_c * _N[_qp];

  // Update stress and strain
  _elastic_strain[_qp] = _mechanical_strain[_qp] - _structured_strain[_qp];
  _stress[_qp] = computeStress(_elastic_strain[_qp]);
  _psin_active[_qp] = _gamma_0 * _c[_qp];
  _psie[_qp] = _stress[_qp].doubleContraction(_elastic_strain[_qp]) / 2;
  _psie_active[_qp] = _psie[_qp] / _g[_qp];
}

Real
ComputeStressWithStrengthSurface::computeReferenceResidual(
    const ADReal & /*effective_trial_stress*/, const ADReal & delta_c)
{
  ADReal sigma = updateTrialState(delta_c);
  return raw_value(sigma - computeStress(delta_c * _N[_qp]).doubleContraction(_N[_qp]));
}

ADReal
ComputeStressWithStrengthSurface::computeResidual(const ADReal & /*effective_trial_stress*/,
                                                  const ADReal & delta_c)
{
  ADReal sigma = updateTrialState(delta_c);
  return sigma - computeStress(delta_c * _N[_qp]).doubleContraction(_N[_qp]) - _g[_qp] * _gamma_0;
}

ADReal
ComputeStressWithStrengthSurface::computeDerivative(const ADReal & /*effective_trial_stress*/,
                                                    const ADReal & delta_c)
{
  ADReal sigma = updateTrialState(delta_c);
  return -computeStress(_N[_qp]).doubleContraction(_N[_qp]) -
         computeStress(delta_c * _N[_qp]).doubleContraction(_N[_qp]) / (2 - delta_c);
}
