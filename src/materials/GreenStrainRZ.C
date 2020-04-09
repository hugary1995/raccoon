//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "GreenStrainRZ.h"

registerADMooseObject("raccoonApp", GreenStrainRZ);

defineADLegacyParams(GreenStrainRZ);

template <ComputeStage compute_stage>
InputParameters
GreenStrainRZ<compute_stage>::validParams()
{
  InputParameters params = ADComputeStrainBase<compute_stage>::validParams();
  params.addClassDescription("Compute Green strain in RZ coordinate.");
  return params;
}

template <ComputeStage compute_stage>
GreenStrainRZ<compute_stage>::GreenStrainRZ(const InputParameters & parameters)
  : ADComputeStrainBase<compute_stage>(parameters),
    _F(declareADProperty<RankTwoTensor>(_base_name + "deformation_gradient"))
{
}

template <ComputeStage compute_stage>
void
GreenStrainRZ<compute_stage>::initQpStatefulProperties()
{
  ADComputeStrainBase<compute_stage>::initQpStatefulProperties();
  _F[_qp].zero();
  _F[_qp].addIa(1.0);
}

template <ComputeStage compute_stage>
void
GreenStrainRZ<compute_stage>::initialSetup()
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
  _disp[2] = &adZeroValue();
  _grad_disp[2] = &adZeroGradient();
}

template <ComputeStage compute_stage>
ADReal
GreenStrainRZ<compute_stage>::computeQpOutOfPlaneGradDisp()
{
  if (!MooseUtils::absoluteFuzzyEqual(_q_point[_qp](0), 0.0))
    return (*_disp[0])[_qp] / _q_point[_qp](0);
  else
    return 0.0;
}

template <ComputeStage compute_stage>
void
GreenStrainRZ<compute_stage>::computeQpProperties()
{
  // deformation gradient
  // F = I + A
  //         A = U_{i,I}, displacement gradient in reference configuration
  ADRankTwoTensor A((*_grad_disp[0])[_qp], (*_grad_disp[1])[_qp], (*_grad_disp[2])[_qp]);
  A(2, 2) = computeQpOutOfPlaneGradDisp();
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
