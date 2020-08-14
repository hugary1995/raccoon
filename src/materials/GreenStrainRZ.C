//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "GreenStrainRZ.h"

registerADMooseObject("raccoonApp", GreenStrainRZ);

InputParameters
GreenStrainRZ::validParams()
{
  InputParameters params = ADComputeStrainBase::validParams();
  params.addClassDescription("Compute Green strain in RZ coordinate.");
  return params;
}

GreenStrainRZ::GreenStrainRZ(const InputParameters & parameters)
  : ADComputeStrainBase(parameters),
    _F(declareADProperty<RankTwoTensor>(_base_name + "deformation_gradient")),
    _F_old(getMaterialPropertyOldByName<RankTwoTensor>(_base_name + "deformation_gradient")),
    _f(declareADProperty<RankTwoTensor>(_base_name + "deformation_gradient_increment"))
{
}

void
GreenStrainRZ::initQpStatefulProperties()
{
  ADComputeStrainBase::initQpStatefulProperties();
  _F[_qp] = ADRankTwoTensor(ADRankTwoTensor::initIdentity);
  _f[_qp] = ADRankTwoTensor(ADRankTwoTensor::initIdentity);
}

void
GreenStrainRZ::initialSetup()
{
  if (getBlockCoordSystem() != Moose::COORD_RZ)
    mooseError("The coordinate system must be set to RZ for Axisymmetric geometries.");

  if (_ndisp != 2)
    paramError("displacements",
               "there must be two displacement variables provided, one in r-direction another in "
               "z-direction");

  // fetch coupled variables and gradients
  for (unsigned int i = 0; i < 2; ++i)
  {
    _disp[i] = &adCoupledValue("displacements", i);
    _grad_disp[i] = &adCoupledGradient("displacements", i);
  }

  // set unused dimensions to zero
  _disp[2] = &_ad_zero;
  _grad_disp[2] = &_ad_grad_zero;
}

ADReal
GreenStrainRZ::computeQpOutOfPlaneGradDisp()
{
  if (!MooseUtils::absoluteFuzzyEqual(_q_point[_qp](0), 0.0))
    return (*_disp[0])[_qp] / _q_point[_qp](0);
  else
    return 0.0;
}

void
GreenStrainRZ::computeProperties()
{
  ADRankTwoTensor ave_F;

  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    // Deformation gradient calculation for 2D problems
    ADRankTwoTensor A((*_grad_disp[0])[_qp], (*_grad_disp[1])[_qp], (*_grad_disp[2])[_qp]);
    A(2, 2) = computeQpOutOfPlaneGradDisp();
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
GreenStrainRZ::computeQpStrain()
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
