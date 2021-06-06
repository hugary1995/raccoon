//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ComputeLowerDimensionalFiniteStrain.h"

#include "libmesh/quadrature.h"

registerMooseObject("raccoonApp", ComputeLowerDimensionalFiniteStrain);

InputParameters
ComputeLowerDimensionalFiniteStrain::validParams()
{
  InputParameters params = ComputeFiniteStrain::validParams();
  params.addClassDescription(
      "Compute a strain increment and rotation increment for finite strains in 2D geometries.");

  params.addRequiredCoupledVar("out_of_plane_strain",
                               "Nonlinear variable to weakly enforce the plane stress condition");
  return params;
}

ComputeLowerDimensionalFiniteStrain::ComputeLowerDimensionalFiniteStrain(
    const InputParameters & parameters)
  : ComputeFiniteStrain(parameters),
    _Fbar(getMaterialPropertyOldByName<RankTwoTensor>(_base_name + "deformation_gradient")),
    _Q(declareProperty<RankTwoTensor>(_base_name + "lower_dimensional_coordinate_transformation")),
    _out_of_plane_strain(coupledValue("out_of_plane_strain"))
{
}

void
ComputeLowerDimensionalFiniteStrain::initQpStatefulProperties()
{
  _deformation_gradient[_qp].setToIdentity();
}

void
ComputeLowerDimensionalFiniteStrain::computeProperties()
{
  RankTwoTensor ave_Fhat;
  Real ave_dfgrd_det = 0.0;

  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {

    // Deformation gradient calculation for 2D problems
    RankTwoTensor A((*_grad_disp[0])[_qp],
                    (*_grad_disp[1])[_qp],
                    (*_grad_disp[2])[_qp]); // Deformation gradient

    // To apply the out-of-plane strain, we follow these steps:

    // Step 1: Compute the coordinate transformation matrix Q such that the z-axis aligns with
    // element normal. Then apply the transformation to matrix A.
    // RealVectorValue nt = getNormal();
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
    RankTwoTensor Ap = _Q[_qp] * A * _Q[_qp].transpose();

    // Step 2: Fill in the out-of-plane strain
    Ap(2, 2) = std::exp(_out_of_plane_strain[_qp]) - 1;

    // Gauss point deformation gradient
    _deformation_gradient[_qp] = Ap;
    _deformation_gradient[_qp].addIa(1.0);

    // Step 3: Transform the coordinate back to the cartesion coordinates.
    _deformation_gradient[_qp] = _Q[_qp].transpose() * _deformation_gradient[_qp] * _Q[_qp];

    // Incremental deformation gradient
    _Fhat[_qp] = _deformation_gradient[_qp] * _Fbar[_qp].inverse();

    if (_volumetric_locking_correction)
    {
      // Calculate average _Fhat for volumetric locking correction
      ave_Fhat += _Fhat[_qp] * _JxW[_qp] * _coord[_qp];

      // Average deformation gradient
      ave_dfgrd_det += _deformation_gradient[_qp].det() * _JxW[_qp] * _coord[_qp];
    }
  }
  if (_volumetric_locking_correction)
  {
    // needed for volumetric locking correction
    ave_Fhat /= _current_elem_volume;
    // average deformation gradient
    ave_dfgrd_det /= _current_elem_volume;
  }
  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    if (_volumetric_locking_correction)
    {
      // Finalize volumetric locking correction
      _Fhat[_qp] *= std::cbrt(ave_Fhat.det() / _Fhat[_qp].det());
      // Volumetric locking correction
      _deformation_gradient[_qp] *= std::cbrt(ave_dfgrd_det / _deformation_gradient[_qp].det());
    }

    computeQpStrain();
  }
}
