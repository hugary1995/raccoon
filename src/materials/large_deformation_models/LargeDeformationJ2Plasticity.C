//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LargeDeformationJ2Plasticity.h"
#include "RaccoonUtils.h"

registerMooseObject("raccoonApp", LargeDeformationJ2Plasticity);

InputParameters
LargeDeformationJ2Plasticity::validParams()
{
  InputParameters params = LargeDeformationPlasticityModel::validParams();
  params.addClassDescription("Large deformation $J_2$ plasticity. The exponential constitutive "
                             "update is used to update the plastic deformation.");
  return params;
}

LargeDeformationJ2Plasticity::LargeDeformationJ2Plasticity(const InputParameters & parameters)
  : LargeDeformationPlasticityModel(parameters)
{
  _check_range = true;
}

void
LargeDeformationJ2Plasticity::updateState(ADRankTwoTensor & stress, ADRankTwoTensor & Fe)
{
  // First assume no plastic increment
  ADReal delta_ep = 0;
  Fe = Fe * _Fp_old[_qp].inverse();
  stress = _elasticity_model->computeMandelStress(Fe);

  // Compute the flow direction following the Prandtl-Reuss flow rule.
  // We guard against zero denominator.
  ADRankTwoTensor stress_dev = stress.deviatoric();
  ADReal stress_dev_norm = stress_dev.doubleContraction(stress_dev);
  if (MooseUtils::absoluteFuzzyEqual(stress_dev_norm, 0))
    stress_dev_norm.value() = libMesh::TOLERANCE * libMesh::TOLERANCE;
  stress_dev_norm = std::sqrt(1.5 * stress_dev_norm);
  _Np[_qp] = 1.5 * stress_dev / stress_dev_norm;
  // Return mapping
  ADReal phi = computeResidual(stress_dev_norm, delta_ep);
  if (phi > 0)
    returnMappingSolve(stress_dev_norm, delta_ep, _console);

  _ep[_qp] = _ep_old[_qp] + delta_ep;
  ADRankTwoTensor delta_Fp = RaccoonUtils::exp(delta_ep * _Np[_qp]);
  _Fp[_qp] = delta_Fp * _Fp_old[_qp];

  // Update stress and energy
  Fe = Fe * delta_Fp.inverse();
  stress = _elasticity_model->computeCauchyStress(Fe);
  _hardening_model->plasticEnergy(_ep[_qp]);

  _heat[_qp] = _hardening_model->plasticDissipation(delta_ep, _ep[_qp], 1) * delta_ep / _dt;

  _heat[_qp] += _hardening_model->thermalConjugate(_ep[_qp]) * delta_ep / _dt;
}

Real
LargeDeformationJ2Plasticity::computeReferenceResidual(const ADReal & effective_trial_stress,
                                                       const ADReal & delta_ep)
{
  return raw_value(effective_trial_stress - _elasticity_model
                                                ->computeMandelStress(delta_ep * _Np[_qp],
                                                                      /*plasticity_update = */ true)
                                                .doubleContraction(_Np[_qp]));
}

ADReal
LargeDeformationJ2Plasticity::computeResidual(const ADReal & effective_trial_stress,
                                              const ADReal & delta_ep)
{
  return effective_trial_stress -
         _elasticity_model
             ->computeMandelStress(delta_ep * _Np[_qp],
                                   /*plasticity_update = */ true)
             .doubleContraction(_Np[_qp]) -
         _hardening_model->plasticEnergy(_ep_old[_qp] + delta_ep, 1) -
         _hardening_model->plasticDissipation(delta_ep, _ep_old[_qp] + delta_ep, 1);
}

ADReal
LargeDeformationJ2Plasticity::computeDerivative(const ADReal & /*effective_trial_stress*/,
                                                const ADReal & delta_ep)
{
  return -_elasticity_model->computeMandelStress(_Np[_qp], /*plasticity_update = */ true)
              .doubleContraction(_Np[_qp]) -
         _hardening_model->plasticEnergy(_ep_old[_qp] + delta_ep, 2) -
         _hardening_model->plasticDissipation(delta_ep, _ep_old[_qp] + delta_ep, 2);
}
