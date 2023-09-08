//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "Function.h"
#include "KLBFNucleationMicroForce.h"

registerADMooseObject("raccoonApp", KLBFNucleationMicroForce);

InputParameters
KLBFNucleationMicroForce::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();

  params.addClassDescription("This class computes the external driving force for nucleation given "
                             "a Drucker-Prager strength envelope developed by Kumar et al. (2020)");

  params.addParam<MaterialPropertyName>(
      "fracture_toughness", "Gc", "energy release rate or fracture toughness");
  params.addParam<MaterialPropertyName>(
      "normalization_constant", "c0", "The normalization constant $c_0$");
  params.addParam<MaterialPropertyName>(
      "regularization_length", "l", "the phase field regularization length");

  params.addParam<MaterialPropertyName>("lambda", "lambda", "Lame's first parameter lambda");
  params.addParam<MaterialPropertyName>("shear_modulus", "G", "shear modulus mu or G");

  params.addRequiredParam<MaterialPropertyName>(
      "tensile_strength", "The tensile strength of the material beyond which the material fails.");

  params.addRequiredParam<MaterialPropertyName>(
      "compressive_strength",
      "The compressive strength of the material beyond which the material fails.");

  params.addRequiredParam<MaterialPropertyName>("delta", "delta");
  params.addParam<MaterialPropertyName>(
      "external_driving_force_name",
      "ex_driving",
      "Name of the material that holds the external_driving_force");
  params.addParam<MaterialPropertyName>(
      "stress_balance_name",
      "stress_balance",
      "Name of the stress balance function $F= \\dfrac{J_2}{\\mu} + \\dfrac{I_1^2}{9\\kappa} - c_e "
      "-\\dfrac{3\\Gc}{8\\delta}=0 $. This value tells how close the material is to stress "
      "surface.");
  params.addParam<MaterialPropertyName>("stress_name", "stress", "Name of the stress tensor");
  return params;
}

KLBFNucleationMicroForce::KLBFNucleationMicroForce(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _ex_driving(declareADProperty<Real>(prependBaseName("external_driving_force_name", true))),
    _Gc(getADMaterialProperty<Real>(prependBaseName("fracture_toughness", true))),
    _c0(getADMaterialProperty<Real>(prependBaseName("normalization_constant", true))),
    _L(getADMaterialProperty<Real>(prependBaseName("regularization_length", true))),
    _lambda(getADMaterialProperty<Real>(prependBaseName("lambda", true))),
    _mu(getADMaterialProperty<Real>(prependBaseName("shear_modulus", true))),
    _sigma_ts(getADMaterialProperty<Real>(prependBaseName("tensile_strength", true))),
    _sigma_cs(getADMaterialProperty<Real>(prependBaseName("compressive_strength", true))),
    _delta(getADMaterialProperty<Real>(prependBaseName("delta", true))),
    _stress(getADMaterialProperty<RankTwoTensor>(prependBaseName("stress_name", true))),
    _stress_balance(declareADProperty<Real>(prependBaseName("stress_balance_name", true)))
{
}

void
KLBFNucleationMicroForce::computeQpProperties()
{
  // The bulk modulus
  ADReal K = _lambda[_qp] + 2 * _mu[_qp] / 3;

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

  // Parameters in the strength surface
  ADReal gamma_0 =
      (_mu[_qp] + 3 * K) * _sigma_ts[_qp] * _L[_qp] / _Gc[_qp] / 18 / _mu[_qp] / _mu[_qp] / K / K;
  ADReal gamma_1 = (1.0 + _delta[_qp]) / (2.0 * _sigma_ts[_qp] * _sigma_cs[_qp]);
  ADReal gamma_2 = (8 * _mu[_qp] + 24 * K - 27 * _sigma_ts[_qp]) / 144 / _mu[_qp] / K;
  ADReal beta_0 = _delta[_qp] * M;
  ADReal beta_1 = (-gamma_1 * M - gamma_2) * (_sigma_cs[_qp] - _sigma_ts[_qp]) -
                  gamma_0 * (std::pow(_sigma_cs[_qp], 3) - std::pow(_sigma_ts[_qp], 3));
  ADReal beta_2 =
      std::sqrt(3.0) * ((-gamma_1 * M + gamma_2) * (_sigma_cs[_qp] + _sigma_ts[_qp]) +
                        gamma_0 * (std::pow(_sigma_cs[_qp], 3) + std::pow(_sigma_ts[_qp], 3)));
  ADReal beta_3 = _L[_qp] * _sigma_ts[_qp] / _mu[_qp] / K / _Gc[_qp];

  // Compute the external driving force required to recover the desired strength envelope.
  _ex_driving[_qp] = (beta_2 * std::sqrt(J2) + beta_1 * I1 + beta_0) / (1 + beta_3 * I1 * I1);

  _stress_balance[_qp] = J2 / _mu[_qp] + std::pow(I1, 2) / 9.0 / K - _ex_driving[_qp] - M;
}
