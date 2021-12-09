//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "Function.h"
#include "GeneralizedExternalDrivingForce.h"

registerADMooseObject("raccoonApp", GeneralizedExternalDrivingForce);

InputParameters GeneralizedExternalDrivingForce::validParams() {
  InputParameters params = Material::validParams();
  params.addClassDescription("computes the Kumar coeffs"
                             "Ce()");
  params.addParam<MaterialPropertyName>("energy_release_rate", "Gc",
                        "energy release rate or fracture toughness");
  params.addParam<MaterialPropertyName>("phase_field_regularization_length", "l",
                        "the phase field regularization length");
  params.addParam<MaterialPropertyName>("Lambda", "Lambda", "Lame's first parameter Lambda");
  params.addParam<MaterialPropertyName>("shear_modulus", "shear modulus mu or G");
  params.addRequiredParam<Real>("tensile_strength",
                                "critical tensile strength");
  params.addRequiredParam<Real>("compressive_strength",
                                "critical compressive strength");
  params.addRequiredParam<Real>("delta", "delta");
  params.addParam<MaterialPropertyName>("rank_two_tensor", "stress",
                                        "name of the stress tensor");
  params.addParam<MaterialPropertyName>(
      "external_driving_force_name", "ex_driving",
      "name of the material that holds the external_driving_force");
  return params;
}

GeneralizedExternalDrivingForce::GeneralizedExternalDrivingForce(
    const InputParameters &parameters)
    : Material(parameters), _ex_driving_name(getParam<MaterialPropertyName>(
                                "external_driving_force_name")),
      _ex_driving(declareADProperty<Real>(_ex_driving_name)),
      _Gc(getADMaterialProperty<Real>("energy_release_rate")),
      _L(getADMaterialProperty<Real>("phase_field_regularization_length")),
      _Lambda(getADMaterialProperty<Real>("Lambda")),
      _mu(getADMaterialProperty<Real>("shear_modulus")),
      _sigma_ts(getParam<Real>("tensile_strength")),
      _sigma_cs(getParam<Real>("compressive_strength")),
      _delta(getParam<Real>("delta")),
      _rank_two_tensor(getParam<MaterialPropertyName>("rank_two_tensor")),
      _stress(getADMaterialProperty<RankTwoTensor>(_rank_two_tensor)),
      _F_surface(declareADProperty<Real>("F_surface")),
      _J2(declareADProperty<Real>("J2")) {}

void GeneralizedExternalDrivingForce::computeQpProperties() {
  ADReal _gamma_0 = _sigma_ts / 6.0 / (3.0 * _Lambda[_qp] + 2.0 * _mu[_qp]) +
                    _sigma_ts / 6.0 / _mu[_qp];
  ADReal _gamma_1 =
      (1.0 + _delta) * (_sigma_cs - _sigma_ts) / (2.0 * _sigma_ts * _sigma_cs);
  ADReal _gamma_2 = (1.0 + _delta) * (_sigma_cs + _sigma_ts) /
                    (2.0 * _sigma_ts * _sigma_cs) * std::sqrt(3.0);
  ADReal _temp = _Gc[_qp] * 3.0 / _L[_qp] / 8.0;
  ADReal I1 = _stress[_qp](0, 0) + _stress[_qp](1, 1) + _stress[_qp](2, 2);
  _J2[_qp] = (pow(_stress[_qp](0, 0) - _stress[_qp](1, 1), 2) +
              pow(_stress[_qp](1, 1) - _stress[_qp](2, 2), 2) +
              pow(_stress[_qp](0, 0) - _stress[_qp](2, 2), 2)) /
                 6.0 +
             pow(_stress[_qp](0, 1), 2) + pow(_stress[_qp](0, 2), 2) +
             pow(_stress[_qp](2, 1), 2);

  mooseAssert(_J2[_qp] >= 0, "Negative J2");

  ADReal _beta_0 = _delta * _temp;
  ADReal _beta_1 = -_gamma_1 * _temp + _gamma_0;
  ADReal _beta_2 = -_gamma_2 * _temp + std::sqrt(3.0) * _gamma_0;
  ADReal _beta_3 = (1 - std::sqrt(I1 * I1) / I1) *
                   (_J2[_qp] / 2.0 / _mu[_qp] +
                    I1 * I1 / 6.0 / (3.0 * _Lambda[_qp] + 2.0 * _mu[_qp]));

  _ex_driving[_qp] =
      _beta_2 * std::sqrt(_J2[_qp]) + _beta_1 * I1 + _beta_0 + _beta_3;
  ADReal K = _Lambda[_qp] + 2.0 * _mu[_qp] / 3.0;
  _F_surface[_qp] =
      _J2[_qp] / _mu[_qp] + I1 * I1 / 9.0 / K - _ex_driving[_qp] - _temp;
}
