//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "TotalPotentialEnergyDensity.h"
#include "metaphysicl/raw_type.h"

registerMooseObject("raccoonApp", TotalPotentialEnergyDensity);

InputParameters
TotalPotentialEnergyDensity::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription(
      "Computes the total potential energy density (stored elastic energy + fracture surface "
      "energy + penalty irreversibility energy) as a plain Real material property. Its domain "
      "integral is the total potential energy whose gradient is the residual, so it serves as a "
      "consistent merit for a custom outer nonlinear-preconditioner line search.");

  params.addRequiredCoupledVar("phase_field", "The phase-field (damage) variable");
  params.addParam<MaterialPropertyName>(
      "strain_energy_density",
      "psie",
      "Stored (degraded) elastic strain energy density, g*psie_active + psie_inactive");
  params.addParam<MaterialPropertyName>(
      "crack_geometric_function", "alpha", "The crack geometric function alpha(d)");
  params.addParam<MaterialPropertyName>("fracture_toughness", "Gc", "The fracture toughness Gc");
  params.addParam<MaterialPropertyName>(
      "normalization_constant", "c0", "The normalization constant c0");
  params.addParam<MaterialPropertyName>(
      "regularization_length", "l", "The phase-field regularization length l");
  params.addParam<Real>(
      "penalty",
      0.0,
      "Penalty coefficient (gamma) for the irreversibility energy (penalty/2)<d-d_old>_-^2. "
      "0 (default) omits it.");
  params.addParam<Real>(
      "viscosity",
      0.0,
      "Viscosity coefficient (eta) for the viscous incremental energy (eta/2/dt)(d-d_old)^2 -- the "
      "potential whose d-gradient is the viscous residual eta*(d-d_old)/dt (ADPFFViscosity). Include "
      "it (matching the kernel) so psi_total stays the consistent merit. 0 (default) omits it.");
  params.addParam<MaterialPropertyName>(
      "total_energy_density", "psi_total", "Name of the computed total potential energy density");

  return params;
}

TotalPotentialEnergyDensity::TotalPotentialEnergyDensity(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _psie(getADMaterialProperty<Real>(prependBaseName("strain_energy_density", true))),
    _alpha(getADMaterialProperty<Real>(prependBaseName("crack_geometric_function", true))),
    _Gc(getADMaterialProperty<Real>(prependBaseName("fracture_toughness", true))),
    _c0(getADMaterialProperty<Real>(prependBaseName("normalization_constant", true))),
    _l(getADMaterialProperty<Real>(prependBaseName("regularization_length", true))),
    _d(coupledValue("phase_field")),
    _grad_d(coupledGradient("phase_field")),
    _d_old(coupledValueOld("phase_field")),
    _penalty(getParam<Real>("penalty")),
    _viscosity(getParam<Real>("viscosity")),
    _psi_total(declareProperty<Real>(prependBaseName("total_energy_density", true)))
{
}

void
TotalPotentialEnergyDensity::computeQpProperties()
{
  const Real psie = MetaPhysicL::raw_value(_psie[_qp]);
  const Real alpha = MetaPhysicL::raw_value(_alpha[_qp]);
  const Real Gc = MetaPhysicL::raw_value(_Gc[_qp]);
  const Real c0 = MetaPhysicL::raw_value(_c0[_qp]);
  const Real l = MetaPhysicL::raw_value(_l[_qp]);

  // Fracture surface energy density = Gc * gamma = Gc/c0 * (alpha/l + l |grad d|^2)
  const Real frac = Gc / c0 * (alpha / l + l * (_grad_d[_qp] * _grad_d[_qp]));

  // Penalty irreversibility energy density = (penalty/2) <d - d_old>_-^2
  const Real jump = _d[_qp] - _d_old[_qp];
  const Real neg = jump < 0.0 ? jump : 0.0;
  const Real pen = 0.5 * _penalty * neg * neg;

  // Viscous incremental energy density = (eta/2/dt)(d - d_old)^2. This is the potential whose
  // d-gradient is the viscous residual eta*(d-d_old)/dt (ADPFFViscosity), so including it keeps
  // psi_total the CONSISTENT merit (grad = the full residual) for the trust-region line search.
  const Real visc = (_viscosity > 0.0 && _dt > 0.0) ? 0.5 * _viscosity / _dt * jump * jump : 0.0;

  _psi_total[_qp] = psie + frac + pen + visc;
}
