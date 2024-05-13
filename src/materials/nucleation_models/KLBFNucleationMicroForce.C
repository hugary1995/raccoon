//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "KLBFNucleationMicroForce.h"

registerMooseObjectReplaced("raccoonApp",
                            KLBFNucleationMicroForce,
                            "12/31/2024 23:59",
                            LDLNucleationMicroForce);

InputParameters
KLBFNucleationMicroForce::validParams()
{
  InputParameters params = NucleationMicroForceBase::validParams();

  params.addClassDescription("This class computes the external driving force for nucleation given "
                             "a Drucker-Prager strength envelope developed by Kumar et al. (2020)");
  params.addRequiredParam<MaterialPropertyName>(
      "tensile_strength", "The tensile strength of the material beyond which the material fails.");
  params.addRequiredParam<MaterialPropertyName>(
      "compressive_strength",
      "The compressive strength of the material beyond which the material fails.");
  params.addRequiredParam<MaterialPropertyName>("delta", "delta");
  return params;
}

KLBFNucleationMicroForce::KLBFNucleationMicroForce(const InputParameters & parameters)
  : NucleationMicroForceBase(parameters),
    _sigma_ts(getADMaterialProperty<Real>(prependBaseName("tensile_strength", true))),
    _sigma_cs(getADMaterialProperty<Real>(prependBaseName("compressive_strength", true))),
    _delta(getADMaterialProperty<Real>(prependBaseName("delta", true)))
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
