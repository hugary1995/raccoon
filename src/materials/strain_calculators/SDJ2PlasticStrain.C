//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SDJ2PlasticStrain.h"

registerMooseObject("raccoonApp", SDJ2PlasticStrain);

InputParameters
SDJ2PlasticStrain::validParams()
{
  InputParameters params = SDPlasticStrainBase::validParams();
  params += ADSingleVariableReturnMappingSolution::validParams();
  params.addClassDescription("Small deformation $J_2$ plasticity. The plastic deformation is "
                             "updated using the additive decompsition of strain.");
  return params;
}

SDJ2PlasticStrain::SDJ2PlasticStrain(const InputParameters & params)
  : SDPlasticStrainBase(params), ADSingleVariableReturnMappingSolution(params)
{
}

void
SDJ2PlasticStrain::computeQpProperties()
{
  // Let the constitutive models know that we are operating on _qp
  _elasticity_model->setQp(_qp);
  _hardening_model->setQp(_qp);

  // Compute the trial elastic strain and trial stress assuming no plastic increment
  ADReal delta_ep = 0;
  ADRankTwoTensor trial_elastic_strain = _mechanical_strain[_qp] - _plastic_strain_old[_qp];
  ADRankTwoTensor trial_stress = _elasticity_model->MandelStress(trial_elastic_strain);

  // Compute the effective trial stress
  ADReal J2 = trial_stress.secondInvariant();
  if (MooseUtils::absoluteFuzzyEqual(J2, 0))
    J2.value() = libMesh::TOLERANCE * libMesh::TOLERANCE;
  ADReal effective_trial_stress = std::sqrt(3) * std::sqrt(J2);

  // Compute the plastic flow direction. It is assumed (and can be proved) that the plastic flow
  // direction does not depend on the plastic strain.
  _Np[_qp] = 1.5 * trial_stress.deviatoric() / effective_trial_stress;

  // Perform the return mapping if the current state is outside the yield surface
  ADReal phi = computeResidual(effective_trial_stress, delta_ep);
  if (phi > 0)
    returnMappingSolve(effective_trial_stress, delta_ep, _console);

  // Update the effective plastic strain, the plastic strain and the elastic strain
  _ep[_qp] = _ep_old[_qp] + delta_ep;
  _plastic_strain[_qp] = _plastic_strain_old[_qp] + delta_ep * _Np[_qp];
}

Real
SDJ2PlasticStrain::computeReferenceResidual(const ADReal & effective_trial_stress,
                                            const ADReal & delta_ep)
{
  return raw_value(
      effective_trial_stress -
      _elasticity_model->MandelStress(delta_ep * _Np[_qp]).doubleContraction(_Np[_qp]));
}

ADReal
SDJ2PlasticStrain::computeResidual(const ADReal & effective_trial_stress, const ADReal & delta_ep)
{
  return effective_trial_stress -
         _elasticity_model->MandelStress(delta_ep * _Np[_qp]).doubleContraction(_Np[_qp]) -
         _hardening_model->flowStress(_ep_old[_qp] + delta_ep);
}

ADReal
SDJ2PlasticStrain::computeDerivative(const ADReal & /*effective_trial_stress*/,
                                     const ADReal & delta_ep)
{
  return -_elasticity_model->MandelStress(_Np[_qp]).doubleContraction(_Np[_qp]) -
         _hardening_model->flowStressDerivative(_ep_old[_qp] + delta_ep);
}
