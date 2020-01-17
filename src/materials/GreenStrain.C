//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "GreenStrain.h"

registerADMooseObject("raccoonApp", GreenStrain);

defineADLegacyParams(GreenStrain);

template <ComputeStage compute_stage>
InputParameters
GreenStrain<compute_stage>::validParams()
{
  InputParameters params = ADComputeStrainBase<compute_stage>::validParams();
  params.addClassDescription("Compute a Green strain.");
  return params;
}

template <ComputeStage compute_stage>
GreenStrain<compute_stage>::GreenStrain(const InputParameters & parameters)
  : ADComputeStrainBase<compute_stage>(parameters),
    _F(declareADProperty<RankTwoTensor>(_base_name + "deformation_gradient"))
{
}

template <ComputeStage compute_stage>
void
GreenStrain<compute_stage>::initQpStatefulProperties()
{
  ADComputeStrainBase<compute_stage>::initQpStatefulProperties();
  _F[_qp].zero();
  _F[_qp].addIa(1.0);
}

template <ComputeStage compute_stage>
void
GreenStrain<compute_stage>::computeQpProperties()
{
  // deformation gradient
  // F = I + A
  //         A = U_{i,I}, displacement gradient in reference configuration
  ADRankTwoTensor A((*_grad_disp[0])[_qp], (*_grad_disp[1])[_qp], (*_grad_disp[2])[_qp]);
  _F[_qp] = A;
  _F[_qp].addIa(1.0);

  // Green strain defined in the reference configuration
  // E = 0.5(F^T F - I)
  ADRankTwoTensor E = _F[_qp].transpose() * _F[_qp];
  E.addIa(-1.0);
  E *= 0.5;

  // total strain defined in the reference configuration
  _total_strain[_qp] = E;
  if (_global_strain)
    _total_strain[_qp] += (*_global_strain)[_qp];

  // mechanical strain in the current configuration
  _mechanical_strain[_qp] = _total_strain[_qp];
  for (auto es : _eigenstrains)
    _mechanical_strain[_qp] -= (*es)[_qp];
}
