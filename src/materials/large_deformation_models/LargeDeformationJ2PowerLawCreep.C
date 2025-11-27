//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LargeDeformationJ2PowerLawCreep.h"
#include "RaccoonUtils.h"

registerMooseObject("raccoonApp", LargeDeformationJ2PowerLawCreep);

InputParameters
LargeDeformationJ2PowerLawCreep::validParams()
{
  InputParameters params = LargeDeformationJ2Plasticity::validParams();
  params.addClassDescription("Large deformation $J_2$ power-law creep. This is an approximation to "
                             "the consistent plasticity model using the update formula $\\epdot "
                             "= A \\dfrac{\\bar{\\sigma}}{\\sigma_y}^n$.");
  params.addRequiredParam<Real>("coefficient",
                                "The coefficient $A$ in the update formula for creep rate");
  params.addRequiredParam<Real>("exponent",
                                "The exponent $n$ in the update formula for creep rate");
  return params;
}

LargeDeformationJ2PowerLawCreep::LargeDeformationJ2PowerLawCreep(const InputParameters & parameters)
  : LargeDeformationJ2Plasticity(parameters),
    _coefficient(getParam<Real>("coefficient")),
    _exponent(getParam<Real>("exponent"))
{
}

ADReal
LargeDeformationJ2PowerLawCreep::computeResidual(const ADReal & effective_trial_stress,
                                                 const ADReal & delta_ep)
{
  using std::pow;
  const ADReal stress_delta =
      effective_trial_stress -
      _elasticity_model->computeMandelStress(delta_ep * _Np[_qp], /*plasticity_update = */ true)
          .doubleContraction(_Np[_qp]);
  const ADReal yield_stress =
      _hardening_model->plasticEnergy(_ep_old[_qp] + delta_ep, 1) +
      _hardening_model->plasticDissipation(delta_ep, _ep_old[_qp] + delta_ep, 1);
  const ADReal creep_rate = _coefficient * pow(stress_delta / yield_stress, _exponent);
  return creep_rate * _dt - delta_ep;
}

ADReal
LargeDeformationJ2PowerLawCreep::computeDerivative(const ADReal & effective_trial_stress,
                                                   const ADReal & delta_ep)
{
  using std::pow;
  const ADReal stress_delta =
      effective_trial_stress -
      _elasticity_model->computeMandelStress(delta_ep * _Np[_qp], /*plasticity_update = */ true)
          .doubleContraction(_Np[_qp]);
  const ADReal dstress_delta_ddelta_ep =
      -_elasticity_model->computeMandelStress(_Np[_qp], /*plasticity_update = */ true)
           .doubleContraction(_Np[_qp]);
  const ADReal yield_stress =
      _hardening_model->plasticEnergy(_ep_old[_qp] + delta_ep, 1) +
      _hardening_model->plasticDissipation(delta_ep, _ep_old[_qp] + delta_ep, 1);
  const ADReal dyield_stress_ddelta_ep =
      _hardening_model->plasticEnergy(_ep_old[_qp] + delta_ep, 2) +
      _hardening_model->plasticDissipation(delta_ep, _ep_old[_qp] + delta_ep, 2);
  const ADReal dcreep_rate =
      _coefficient * _exponent * pow(stress_delta / yield_stress, _exponent - 1);
  return dcreep_rate *
             (dstress_delta_ddelta_ep * yield_stress - stress_delta * dyield_stress_ddelta_ep) /
             yield_stress / yield_stress * _dt -
         1;
}
