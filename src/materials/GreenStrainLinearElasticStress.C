#include "GreenStrainLinearElasticStress.h"

registerMooseObject("raccoonApp", GreenStrainLinearElasticStress);

template <>
InputParameters
validParams<GreenStrainLinearElasticStress>()
{
  InputParameters params = validParams<ComputeStressBase>();
  params.addClassDescription(
      "Compute second piola kirchhoff stress using elasticity for green strains");
  return params;
}

GreenStrainLinearElasticStress::GreenStrainLinearElasticStress(const InputParameters & parameters)
  : ComputeStressBase(parameters),
    _elasticity_tensor(getMaterialPropertyByName<RankFourTensor>(_base_name + "elasticity_tensor"))
{
}

void
GreenStrainLinearElasticStress::computeQpStress()
{
  // second piola kirchhoff stress
  _stress[_qp] = _elasticity_tensor[_qp] * _mechanical_strain[_qp];

  // elastic strain in current configuration
  _elastic_strain[_qp] = _mechanical_strain[_qp];

  // dstress_dstrain
  _Jacobian_mult[_qp] = _elasticity_tensor[_qp];
}
