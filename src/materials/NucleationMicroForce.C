//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu
///
// Aditya Kumar, Blaise Bourdin, Gilles A. Francfort, Oscar Lopez-Pamies,
// Revisiting nucleation in the phase-field approach to brittle fracture,
// Journal of the Mechanics and Physics of Solids,
// Volume 142,
// 2020,
// 104027,
// ISSN 0022-5096,
// https://doi.org/10.1016/j.jmps.2020.104027.
// (https://www.sciencedirect.com/science/article/pii/S0022509620302623)

#include "Function.h"
#include "NucleationMicroForce.h"

registerADMooseObject("raccoonApp", NucleationMicroForce);

InputParameters NucleationMicroForce::validParams() {
  InputParameters params = Material::validParams();
  params.addClassDescription("computes the external driving force for nucleation given a strength envelope. The implementation follows Kumar et. al. (2020).");
  params.addParam<MaterialPropertyName>(
      "fracture_toughness", "Gc", "energy release rate or fracture toughness");
  params.addParam<MaterialPropertyName>(
      "normalization_constant", "c0", "The normalization constant $c_0$");
  params.addParam<MaterialPropertyName>(
      "regularization_length", "l",
      "the phase field regularization length");
  params.addParam<MaterialPropertyName>("lambda", "lambda",
                                        "Lame's first parameter lambda");
  params.addParam<MaterialPropertyName>("shear_modulus", "G",
                                        "shear modulus mu or G");
  params.addRequiredParam<Real>("tensile_strength",
                                "The tensile strength of the material beyond which the material fails.");
  params.addRequiredParam<Real>("compressive_strength",
                                "The compressive strength of the material beyond which the material fails.");
  params.addRequiredParam<Real>("delta", "delta");
  params.addParam<MaterialPropertyName>(
      "external_driving_force_name", "ex_driving",
      "name of the material that holds the external_driving_force");
  return params;
}

NucleationMicroForce::NucleationMicroForce(
    const InputParameters &parameters)
    : Material(parameters), _ex_driving_name(getParam<MaterialPropertyName>(
                                "external_driving_force_name")),
      _ex_driving(declareADProperty<Real>(_ex_driving_name)),
      _Gc(getADMaterialProperty<Real>("fracture_toughness")),
      _c0(getADMaterialProperty<Real>("normalization_constant")),
      _L(getADMaterialProperty<Real>("regularization_length")),
      _lambda(getADMaterialProperty<Real>("lambda")),
      _mu(getADMaterialProperty<Real>("shear_modulus")),
      _sigma_ts(getParam<Real>("tensile_strength")),
      _sigma_cs(getParam<Real>("compressive_strength")),
      _delta(getParam<Real>("delta")),
      _stress(getADMaterialProperty<RankTwoTensor>("stress")) {}

void NucleationMicroForce::computeQpProperties() {
  ADReal K = _lambda[_qp] + 2.0 * _mu[_qp] / 3.0;
  ADReal _gamma_0 = (_mu[_qp] + 3 * K) * _sigma_ts * _L[_qp] / _Gc[_qp] / 18 /
                    _mu[_qp] / _mu[_qp] / K / K;
  ADReal _gamma_1 = (1.0 + _delta) / (2.0 * _sigma_ts * _sigma_cs);
  ADReal _gamma_2 =
      (8 * _mu[_qp] + 24 * K - 27 * _sigma_ts) / 144 / _mu[_qp] / K;
  ADReal _temp = _Gc[_qp] / _L[_qp] / _c0[_qp];
  ADReal I1 = _stress[_qp].trace();
  ADRankTwoTensor stress_dev = _stress[_qp].deviatoric();
  ADReal J2 = stress_dev.doubleContraction(stress_dev);

  mooseAssert(J2 >= 0, "Negative J2");

  ADReal _beta_0 = _delta * _temp;
  ADReal _beta_1 = (-_gamma_1 * _temp - _gamma_2) * (_sigma_cs - _sigma_ts) -
                   _gamma_0 * (pow(_sigma_cs, 3) - pow(_sigma_ts, 3));
  ADReal _beta_2 = std::sqrt(3.0) *
                   ((-_gamma_1 * _temp + _gamma_2) * (_sigma_cs + _sigma_ts) +
                    _gamma_0 * (pow(_sigma_cs, 3) + pow(_sigma_ts, 3)));
  ADReal _beta_3 = _L[_qp] * _sigma_ts / _mu[_qp] / K / _Gc[_qp];
  _ex_driving[_qp] = (_beta_2 * std::sqrt(J2) + _beta_1 * I1 + _beta_0) /
                     (1 + _beta_3 * I1 * I1);
}
