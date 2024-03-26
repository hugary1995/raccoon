//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "Function.h"
#include "MooseMesh.h"
#include "LDLNucleationMicroForce.h"

registerADMooseObject("raccoonApp", LDLNucleationMicroForce);

InputParameters
LDLNucleationMicroForce::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();

  params.addClassDescription(
      "This class computes the external driving force for nucleation given "
      "a Drucker-Prager strength envelope developed by Larsen et al. (2024)");

  params.addParam<MaterialPropertyName>(
      "fracture_toughness", "Gc", "energy release rate or fracture toughness");
  params.addParam<MaterialPropertyName>(
      "normalization_constant", "c0", "The normalization constant $c_0$");
  params.addRequiredCoupledVar("phase_field", "Name of the phase-field (damage) variable");
  params.addParam<MaterialPropertyName>("degradation_function", "g", "The degradation function");
  params.addParam<MaterialPropertyName>(
      "regularization_length", "l", "the phase field regularization length");

  params.addParam<MaterialPropertyName>("lambda", "lambda", "Lame's first parameter lambda");
  params.addParam<MaterialPropertyName>("shear_modulus", "G", "shear modulus mu or G");

  params.addRequiredParam<MaterialPropertyName>(
      "tensile_strength", "The tensile strength of the material beyond which the material fails.");

  params.addRequiredParam<MaterialPropertyName>(
      "hydrostatic_strength",
      "The hydrostatic strength of the material beyond which the material fails.");

  params.addParam<MaterialPropertyName>("delta", "delta", "Name of the unitless coefficient delta");
  params.addParam<bool>("h_correction", false, "Whether to use h correction formula for delta");
  params.addParam<MaterialPropertyName>(
      "external_driving_force_name",
      "ex_driving",
      "Name of the material that holds the external_driving_force");
  params.addParam<MaterialPropertyName>(
      "stress_balance_name",
      "stress_balance",
      "Name of the stress balance function $F= \\dfrac{J_2}{\\mu} + \\dfrac{I_1^2}{9\\kappa} - c_e "
      "-\\dfrac{3\\Gc}{8\\delta}=0 $. This value tells how close the material is to strength "
      "envelope.");
  params.addParam<MaterialPropertyName>("stress_name", "stress", "Name of the stress tensor");
  return params;
}

LDLNucleationMicroForce::LDLNucleationMicroForce(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _ex_driving(declareADProperty<Real>(prependBaseName("external_driving_force_name", true))),
    _Gc(getADMaterialProperty<Real>(prependBaseName("fracture_toughness", true))),
    _d(coupledValue("phase_field")),
    _g_name(prependBaseName("degradation_function", true)),
    _g(getADMaterialProperty<Real>(_g_name)),
    _c0(getADMaterialProperty<Real>(prependBaseName("normalization_constant", true))),
    _L(getADMaterialProperty<Real>(prependBaseName("regularization_length", true))),
    _lambda(getADMaterialProperty<Real>(prependBaseName("lambda", true))),
    _mu(getADMaterialProperty<Real>(prependBaseName("shear_modulus", true))),
    _sigma_ts(getADMaterialProperty<Real>(prependBaseName("tensile_strength", true))),
    _sigma_hs(getADMaterialProperty<Real>(prependBaseName("hydrostatic_strength", true))),
    _delta(declareADProperty<Real>(prependBaseName("delta", true))),
    _h_correction(getParam<bool>("h_correction")),
    _stress(getADMaterialProperty<RankTwoTensor>(prependBaseName("stress_name", true))),
    _stress_balance(declareADProperty<Real>(prependBaseName("stress_balance_name", true)))
{
}

void
LDLNucleationMicroForce::computeQpProperties()
{
  // The bulk modulus
  ADReal K = _lambda[_qp] + 2.0 * _mu[_qp] / 3.0;

  // The Young's modulus
  ADReal E = 9.0 * _mu[_qp] * K / (_mu[_qp] + 3.0 * K);

  // The mobility
  ADReal M = _Gc[_qp] / _L[_qp] / _c0[_qp];

  // Invariants of the stress
  ADReal I1 = _stress[_qp].trace();
  ADRankTwoTensor stress_dev = _stress[_qp].deviatoric();
  ADReal J2 = 0.5 * stress_dev.doubleContraction(stress_dev);

  // Just to be extra careful... J2 is for sure non-negative but descritization and interpolation
  // might bring surprise
  mooseAssert(J2 >= 0, "Negative J2");

  // define zero J2's derivative
  if (MooseUtils::absoluteFuzzyEqual(J2, 0))
    J2.value() = libMesh::TOLERANCE * libMesh::TOLERANCE;

  // Compute critical energy
  ADReal W_ts = _sigma_ts[_qp] * _sigma_ts[_qp] / 2.0 / E;
  ADReal W_hs = _sigma_hs[_qp] * _sigma_hs[_qp] / 2.0 / K;

  // Compute delta
  if (!_h_correction)
  {
    // Use formula without h correction
    _delta[_qp] = (_sigma_ts[_qp] + (1 + 2 * std::sqrt(3)) * _sigma_hs[_qp]) /
                      (8 + 3 * std::sqrt(3)) / _sigma_hs[_qp] * 3.0 / 16.0 *
                      (_Gc[_qp] / W_ts / _L[_qp]) +
                  3.0 / 8.0;
  }
  else
  {
    // Get mesh size of current element
    ADReal h = _current_elem->hmin();

    // Use formula with h correction
    _delta[_qp] = std::pow(1 + 3.0 / 8.0 * h / _L[_qp], -2) *
                      (_sigma_ts[_qp] + (1 + 2 * std::sqrt(3.0)) * _sigma_hs[_qp]) /
                      (8 + 3 * std::sqrt(3.0)) / _sigma_hs[_qp] * 3 / 16 *
                      (_Gc[_qp] / W_ts / _L[_qp]) +
                  std::pow(1 + 3.0 / 8.0 * h / _L[_qp], -1) * 2 / 5;
  }

  // Parameters in the strength surface
  ADReal alpha_1 =
      -_delta[_qp] * _Gc[_qp] / 8.0 / _sigma_hs[_qp] / _L[_qp] + 2.0 / 3.0 * W_hs / _sigma_hs[_qp];
  ADReal alpha_2 = -(std::sqrt(3.0) / 8.0 * _delta[_qp] * (3.0 * _sigma_hs[_qp] - _sigma_ts[_qp]) /
                         (_sigma_hs[_qp] * _sigma_ts[_qp]) * _Gc[_qp] / _L[_qp] +
                     2.0 / std::sqrt(3.0) * W_hs / _sigma_hs[_qp] -
                     2.0 * std::sqrt(3.0) * W_ts / _sigma_ts[_qp]);

  // Compute the external driving force required to recover the desired strength envelope.
  _ex_driving[_qp] =
      alpha_2 * std::sqrt(J2) + alpha_1 * I1 +
      (1.0 - std::sqrt(I1 * I1) / I1) / std::pow(_g[_qp], 1.5) *
          (J2 / 2.0 / _mu[_qp] + I1 * I1 / 6.0 / (3.0 * _lambda[_qp] + 2.0 * _mu[_qp]));

  _stress_balance[_qp] =
      J2 / _mu[_qp] + std::pow(I1, 2) / 9.0 / K - _ex_driving[_qp] - M * _delta[_qp];
}