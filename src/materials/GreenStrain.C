#include "GreenStrain.h"

registerMooseObject("raccoonApp", GreenStrain);

template <>
InputParameters
validParams<GreenStrain>()
{
  InputParameters params = validParams<ComputeStrainBase>();
  params.addClassDescription("Compute Green strain.");
  return params;
}

GreenStrain::GreenStrain(const InputParameters & parameters)
  : ComputeStrainBase(parameters),
    _F(declareProperty<RankTwoTensor>(_base_name + "deformation_gradient"))
{
}

void
GreenStrain::computeQpProperties()
{
  // F = I + A
  //         A = U_{i,I}, displacement gradient in reference configuration
  RankTwoTensor A((*_grad_disp[0])[_qp], (*_grad_disp[1])[_qp], (*_grad_disp[2])[_qp]);
  _F[_qp] = A;
  _F[_qp].addIa(1.0);

  // E = 0.5(F^T F - I)
  RankTwoTensor E = _F[_qp].transpose() * _F[_qp];
  E.addIa(-1.0);
  E *= 0.5;

  // total strain
  _total_strain[_qp] = E;
  if (_global_strain)
    _total_strain[_qp] += (*_global_strain)[_qp];

  // mechanical strain
  _mechanical_strain[_qp] = _total_strain[_qp];
  for (auto es : _eigenstrains)
    _mechanical_strain[_qp] -= (*es)[_qp];
}
