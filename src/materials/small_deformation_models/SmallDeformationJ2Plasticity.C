//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SmallDeformationJ2Plasticity.h"

registerMooseObject("raccoonApp", SmallDeformationJ2Plasticity);

InputParameters
SmallDeformationJ2Plasticity::validParams()
{
  InputParameters params = SmallDeformationPlasticityModel::validParams();
  return params;
}

SmallDeformationJ2Plasticity::SmallDeformationJ2Plasticity(const InputParameters & parameters)
  : SmallDeformationPlasticityModel(parameters)
{
}

void
SmallDeformationJ2Plasticity::updateState(ADRankTwoTensor & stress,
                                          ADRankTwoTensor & elastic_strain)
{
  // First assume no plastic increment
  ADReal delta_ep = 0;
  elastic_strain -= _plastic_strain_old[_qp];
  stress = _elasticity_model->computeStress(elastic_strain);

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
  _plastic_strain[_qp] = _plastic_strain_old[_qp] + delta_ep * _Np[_qp];

  // Update stress
  elastic_strain -= delta_ep * _Np[_qp];
  stress = _elasticity_model->computeStress(elastic_strain);
  _hardening_model->plasticEnergy(_ep[_qp]);
}

Real
SmallDeformationJ2Plasticity::computeReferenceResidual(const ADReal & effective_trial_stress,
                                                       const ADReal & delta_ep)
{
  return raw_value(
      effective_trial_stress -
      _elasticity_model->computeStress(delta_ep * _Np[_qp]).doubleContraction(_Np[_qp]));
}

ADReal
SmallDeformationJ2Plasticity::computeResidual(const ADReal & effective_trial_stress,
                                              const ADReal & delta_ep)
{
  return effective_trial_stress -
         _elasticity_model->computeStress(delta_ep * _Np[_qp]).doubleContraction(_Np[_qp]) -
         _hardening_model->plasticEnergy(_ep_old[_qp] + delta_ep, 1);
}

ADReal
SmallDeformationJ2Plasticity::computeDerivative(const ADReal & /*effective_trial_stress*/,
                                                const ADReal & delta_ep)
{
  return -_elasticity_model->computeStress(_Np[_qp]).doubleContraction(_Np[_qp]) -
         _hardening_model->plasticEnergy(_ep_old[_qp] + delta_ep, 2);
}
