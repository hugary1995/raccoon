#include "GreenStrainLinearElasticStress.h"

registerMooseObject("raccoonApp", GreenStrainLinearElasticStress);

template <>
InputParameters
validParams<GreenStrainLinearElasticStress>()
{
  InputParameters params = validParams<ComputeStressBase>();
  params.addClassDescription(
      "Compute second piola kirchhoff stress using elasticity for green strains");
  params.addParam<bool>(
      "compute_cauchy_stress", false, "whether to compute the stress on current configuration");

  return params;
}

GreenStrainLinearElasticStress::GreenStrainLinearElasticStress(const InputParameters & parameters)
  : ComputeStressBase(parameters),
    _elasticity_tensor(getMaterialPropertyByName<RankFourTensor>(_base_name + "elasticity_tensor")),
    _cauchy_stress(getParam<bool>("compute_cauchy_stress")
                       ? &declareProperty<RankTwoTensor>(_base_name + "cauchy_stress")
                       : NULL),
    _deformation_gradient(
        getParam<bool>("compute_cauchy_stress")
            ? &getMaterialPropertyByName<RankTwoTensor>(_base_name + "deformation_gradient")
            : NULL)
{
}

void
GreenStrainLinearElasticStress::computeQpStress()
{
  // second piola kirchhoff stress
  _stress[_qp] = _elasticity_tensor[_qp] * _mechanical_strain[_qp];

  // elastic strain in reference configuration
  _elastic_strain[_qp] = _mechanical_strain[_qp];

  // dstress_dstrain
  _Jacobian_mult[_qp] = _elasticity_tensor[_qp];

  // compute cauchy stress per request
  if (_cauchy_stress)
    (*_cauchy_stress)[_qp] = (*_deformation_gradient)[_qp] * _stress[_qp] *
                             (*_deformation_gradient)[_qp].transpose() /
                             (*_deformation_gradient)[_qp].det();
}
