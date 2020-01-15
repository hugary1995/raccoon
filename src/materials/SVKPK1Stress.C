//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SVKPK1Stress.h"

registerADMooseObject("raccoonApp", SVKPK1Stress);

defineADLegacyParams(SVKPK1Stress);

template <ComputeStage compute_stage>
InputParameters
SVKPK1Stress<compute_stage>::validParams()
{
  InputParameters params = ADComputeStressBase<compute_stage>::validParams();
  params.addClassDescription("Compute stress using the St. Venant Kirchhoff hyperelastic model");
  params.addParam<bool>(
      "cauchy_stress", false, "whether to output the stress on current configuration");
  return params;
}

template <ComputeStage compute_stage>
SVKPK1Stress<compute_stage>::SVKPK1Stress(const InputParameters & parameters)
  : ADComputeStressBase<compute_stage>(parameters),
    _elasticity_tensor(getADMaterialProperty<RankFourTensor>(_base_name + "elasticity_tensor")),
    _F(getADMaterialProperty<RankTwoTensor>(_base_name + "deformation_gradient")),
    _cauchy_stress(getParam<bool>("cauchy_stress")
                       ? &declareADProperty<RankTwoTensor>(_base_name + "cauchy_stress")
                       : nullptr)
{
}

template <ComputeStage compute_stage>
void
SVKPK1Stress<compute_stage>::computeQpStress()
{
  // Isotropic elasticity is assumed and should be enforced
  // P = F * S = F * (C:E)
  _stress[_qp] = _F[_qp] * (_elasticity_tensor[_qp] * _mechanical_strain[_qp]);

  // elastic strain in reference configuration
  _elastic_strain[_qp] = _mechanical_strain[_qp];

  // compute cauchy stress per request
  if (_cauchy_stress)
    (*_cauchy_stress)[_qp] = _stress[_qp] * _F[_qp].transpose() / _F[_qp].det();
}
