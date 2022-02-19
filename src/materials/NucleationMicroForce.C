//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "Function.h"
#include "NucleationMicroForce.h"

registerADMooseObject("raccoonApp", NucleationMicroForce);

InputParameters
NucleationMicroForce::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();

  params.addClassDescription(
      "This class computes the external driving force for nucleation given "
      "a Drucker-Prager strength envelope. The implementation follows Kumar et. al. (2020).");

  params.addParam<MaterialPropertyName>(
      "fracture_toughness", "Gc", "energy release rate or fracture toughness");
  params.addParam<MaterialPropertyName>(
      "normalization_constant", "c0", "The normalization constant $c_0$");
  params.addParam<MaterialPropertyName>(
      "regularization_length", "l", "the phase field regularization length");

  params.addParam<MaterialPropertyName>("lambda", "lambda", "Lame's first parameter lambda");
  params.addParam<MaterialPropertyName>("shear_modulus", "G", "shear modulus mu or G");

  params.addRequiredParam<Real>(
      "tensile_strength", "The tensile strength of the material beyond which the material fails.");
  params.addRequiredParam<Real>(
      "compressive_strength",
      "The compressive strength of the material beyond which the material fails.");

  params.addRequiredParam<Real>("delta", "delta");
  params.addParam<MaterialPropertyName>(
      "external_driving_force_name",
      "ex_driving",
      "Name of the material that holds the external_driving_force");

  return params;
}

NucleationMicroForce::NucleationMicroForce(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _ex_driving(declareADProperty<Real>(prependBaseName("external_driving_force_name", true))),
    _Gc(getADMaterialProperty<Real>(prependBaseName("fracture_toughness", true))),
    _c0(getADMaterialProperty<Real>(prependBaseName("normalization_constant", true))),
    _L(getADMaterialProperty<Real>(prependBaseName("regularization_length", true))),
    _lambda(getADMaterialProperty<Real>(prependBaseName("lambda", true))),
    _mu(getADMaterialProperty<Real>(prependBaseName("shear_modulus", true))),
    _sigma_ts(getParam<Real>("tensile_strength")),
    _sigma_cs(getParam<Real>("compressive_strength")),
    _delta(getParam<Real>("delta")),
    _stress(getADMaterialProperty<RankTwoTensor>(prependBaseName("stress")))
{
}

void
NucleationMicroForce::computeQpProperties()
{
  // The bulk modulus
  ADReal K = _lambda[_qp] + 2 * _mu[_qp] / 3;

  // Parameters in the strength surface
  ADReal gamma_0 =
      (_mu[_qp] + 3 * K) * _sigma_ts * _L[_qp] / _Gc[_qp] / 18 / _mu[_qp] / _mu[_qp] / K / K;
  ADReal gamma_1 = (1.0 + _delta) / (2.0 * _sigma_ts * _sigma_cs);
  ADReal gamma_2 = (8 * _mu[_qp] + 24 * K - 27 * _sigma_ts) / 144 / _mu[_qp] / K;

  // The mobility
  ADReal M = _Gc[_qp] / _L[_qp] / _c0[_qp];

  // Invariants of the stress
  ADReal I1 = _stress[_qp].trace();
  ADRankTwoTensor stress_dev = _stress[_qp].deviatoric();
  ADReal J2 = 0.5 * stress_dev.doubleContraction(stress_dev);

  // Just to be extra careful... J2 is for sure non-negative.
  mooseAssert(J2 >= 0, "Negative J2");

  // define zero J2's derivative
  if (MooseUtils::absoluteFuzzyEqual(J2, 0))
    J2.value() = libMesh::TOLERANCE * libMesh::TOLERANCE;

  // Compute the external driving force required to recover the desired strength envelope.
  ADReal beta_0 = _delta * M;
  ADReal beta_1 = (-gamma_1 * M - gamma_2) * (_sigma_cs - _sigma_ts) -
                  gamma_0 * (pow(_sigma_cs, 3) - pow(_sigma_ts, 3));
  ADReal beta_2 = std::sqrt(3.0) * ((-gamma_1 * M + gamma_2) * (_sigma_cs + _sigma_ts) +
                                    gamma_0 * (pow(_sigma_cs, 3) + pow(_sigma_ts, 3)));
  ADReal beta_3 = _L[_qp] * _sigma_ts / _mu[_qp] / K / _Gc[_qp];
  _ex_driving[_qp] = (beta_2 * std::sqrt(J2) + beta_1 * I1 + beta_0) / (1 + beta_3 * I1 * I1);
}
