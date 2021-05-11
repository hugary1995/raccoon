//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "GreenStrain.h"

registerMooseObject("raccoonApp", GreenStrain);

InputParameters
GreenStrain::validParams()
{
  InputParameters params = ADComputeStrainBase::validParams();
  params.addClassDescription("Compute Green strain in RZ coordinate.");
  return params;
}

GreenStrain::GreenStrain(const InputParameters & parameters)
  : ADComputeStrainBase(parameters),
    _F(declareADProperty<RankTwoTensor>(_base_name + "deformation_gradient")),
    _F_old(getMaterialPropertyOldByName<RankTwoTensor>(_base_name + "deformation_gradient")),
    _f(declareADProperty<RankTwoTensor>(_base_name + "deformation_gradient_increment"))
{
}

void
GreenStrain::initQpStatefulProperties()
{
  ADComputeStrainBase::initQpStatefulProperties();
  _F[_qp] = ADRankTwoTensor(ADRankTwoTensor::initIdentity);
  _f[_qp] = ADRankTwoTensor(ADRankTwoTensor::initIdentity);
}

void
GreenStrain::computeProperties()
{
  ADRankTwoTensor ave_F;

  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    ADRankTwoTensor A((*_grad_disp[0])[_qp], (*_grad_disp[1])[_qp], (*_grad_disp[2])[_qp]);
    _F[_qp] = A;
    _F[_qp].addIa(1.0);

    if (_volumetric_locking_correction)
      ave_F += _F[_qp] * _JxW[_qp] * _coord[_qp];
  }

  if (_volumetric_locking_correction)
    ave_F /= _current_elem_volume;

  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    if (_volumetric_locking_correction)
      _F[_qp] *= std::cbrt(ave_F.det() / _F[_qp].det());

    _f[_qp] = _F[_qp] * _F_old[_qp].inverse();
    computeQpStrain();
  }
}

void
GreenStrain::computeQpStrain()
{
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
