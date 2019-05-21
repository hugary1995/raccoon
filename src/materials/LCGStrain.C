#include "LCGStrain.h"

registerADMooseObject("raccoonApp", LCGStrain);

defineADValidParams(LCGStrain,
                    ADComputeStrainBase,
                    params.addClassDescription("Compute the left Cauchy-Green strain."););

template <ComputeStage compute_stage>
LCGStrain<compute_stage>::LCGStrain(const InputParameters & parameters)
  : ADComputeStrainBase<compute_stage>(parameters),
    _F(adDeclareADProperty<RankTwoTensor>(_base_name + "deformation_gradient"))
{
}

template <ComputeStage compute_stage>
void
LCGStrain<compute_stage>::computeQpProperties()
{
  // deformation gradient
  // F = I + A
  //         A = U_{i,I}, displacement gradient in reference configuration
  ADRankTwoTensor A((*_grad_disp[0])[_qp], (*_grad_disp[1])[_qp], (*_grad_disp[2])[_qp]);
  _F[_qp] = A;
  _F[_qp].addIa(1.0);

  // Green strain defined in the reference configuration
  // E = 0.5(F^T F - I)
  ADRankTwoTensor b = _F[_qp] * _F[_qp].transpose();

  // total strain defined in the current configuration
  // e = F E F^T / det(F)
  _total_strain[_qp] = b;
  if (_global_strain)
    _total_strain[_qp] += (*_global_strain)[_qp];

  // mechanical strain in the current configuration
  _mechanical_strain[_qp] = _total_strain[_qp];
  for (auto es : _eigenstrains)
    _mechanical_strain[_qp] -= (*es)[_qp];
}
