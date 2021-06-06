//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADComputeLowerDimensionalFiniteStrain.h"

#include "libmesh/quadrature.h"

registerMooseObject("raccoonApp", ADComputeLowerDimensionalFiniteStrain);

InputParameters
ADComputeLowerDimensionalFiniteStrain::validParams()
{
  InputParameters params = ADComputeFiniteStrain::validParams();
  params.addClassDescription(
      "Compute a strain increment and rotation increment for finite strains in 2D geometries.");

  params.addRequiredCoupledVar("out_of_plane_strain",
                               "Nonlinear variable to weakly enforce the plane stress condition");
  return params;
}

ADComputeLowerDimensionalFiniteStrain::ADComputeLowerDimensionalFiniteStrain(
    const InputParameters & parameters)
  : ADComputeFiniteStrain(parameters),
    _Q(declareProperty<RankTwoTensor>(_base_name + "lower_dimensional_coordinate_transformation")),
    _out_of_plane_strain(adCoupledValue("out_of_plane_strain")),
    _out_of_plane_strain_old(coupledValueOld("out_of_plane_strain"))
{
}

void
ADComputeLowerDimensionalFiniteStrain::computeProperties()
{
  ADRankTwoTensor ave_Fhat;

  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    RealVectorValue nt(-_q_point[_qp](0), -_q_point[_qp](1), 0);
    nt /= nt.norm();
    RealVectorValue nz(0, 0, 1);
    RealVectorValue n = nz.cross(nt);
    Real cost = nt * nz;
    Real sint = std::sqrt(1 - cost * cost);
    RankTwoTensor nxn, offn;
    nxn.vectorOuterProduct(n, n);
    offn.fillFromInputVector({0, -n(2), n(1), n(2), 0, -n(0), -n(1), n(0), 0});
    _Q[_qp].setToIdentity();
    _Q[_qp] *= cost;
    _Q[_qp] += nxn * (1 - cost) + offn * sint;

    // Deformation gradient calculation for 2D problems
    ADRankTwoTensor A((*_grad_disp[0])[_qp],
                      (*_grad_disp[1])[_qp],
                      (*_grad_disp[2])[_qp]); // Deformation gradient
    RankTwoTensor Fbar((*_grad_disp_old[0])[_qp],
                       (*_grad_disp_old[1])[_qp],
                       (*_grad_disp_old[2])[_qp]); // Old Deformation gradient

    ADRankTwoTensor Ap = _Q[_qp] * A * _Q[_qp].transpose();
    RankTwoTensor Fbarp = _Q[_qp] * Fbar * _Q[_qp].transpose();

    // Compute the displacement gradient for the out of plane direction for plane strain,
    // generalized plane strain, or axisymmetric problems
    Ap(2, 2) = std::exp(_out_of_plane_strain[_qp]) - 1;
    Ap(2, 0) = Ap(2, 1) = Ap(0, 2) = Ap(1, 2) = 0;
    Fbarp(2, 2) = std::exp(_out_of_plane_strain_old[_qp]) - 1;
    Fbarp(2, 0) = Fbarp(2, 1) = Fbarp(0, 2) = Fbarp(1, 2) = 0;

    Ap -= Fbarp; // very nearly A = gradU - gradUold

    Fbarp.addIa(1.0); // Fbar = ( I + gradUold)

    // Incremental deformation gradient _Fhat = I + A Fbar^-1
    _Fhat[_qp] = Ap * Fbarp.inverse();
    _Fhat[_qp].addIa(1.0);
    _Fhat[_qp] = _Q[_qp].transpose() * _Fhat[_qp] * _Q[_qp];

    // Calculate average _Fhat for volumetric locking correction
    if (_volumetric_locking_correction)
      ave_Fhat += _Fhat[_qp] * _JxW[_qp] * _coord[_qp];
  }

  if (_volumetric_locking_correction)
    ave_Fhat /= _current_elem_volume;

  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    // Finalize volumetric locking correction
    if (_volumetric_locking_correction)
      _Fhat[_qp] *= std::cbrt(ave_Fhat.det() / _Fhat[_qp].det());

    computeQpStrain();
  }
}
