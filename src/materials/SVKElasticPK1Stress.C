//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SVKElasticPK1Stress.h"

registerADMooseObject("raccoonApp", SVKElasticPK1Stress);

InputParameters
SVKElasticPK1Stress::validParams()
{
  InputParameters params = ADComputeStressBase::validParams();
  params.addClassDescription("Compute stress using the St. Venant Kirchhoff hyperelastic model");
  params.addParam<bool>(
      "cauchy_stress", false, "whether to output the stress on current configuration");
  return params;
}

SVKElasticPK1Stress::SVKElasticPK1Stress(const InputParameters & parameters)
  : ADComputeStressBase(parameters),
    _elasticity_tensor(getADMaterialProperty<RankFourTensor>(_base_name + "elasticity_tensor")),
    _F(getADMaterialProperty<RankTwoTensor>(_base_name + "deformation_gradient")),
    _cauchy_stress(getParam<bool>("cauchy_stress")
                       ? &declareADProperty<RankTwoTensor>(_base_name + "cauchy_stress")
                       : nullptr)
{
}

void
SVKElasticPK1Stress::computeQpStress()
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
