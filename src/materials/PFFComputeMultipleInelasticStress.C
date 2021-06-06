//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PFFComputeMultipleInelasticStress.h"

registerMooseObject("raccoonApp", PFFComputeMultipleInelasticStress);

InputParameters
PFFComputeMultipleInelasticStress::validParams()
{
  InputParameters params = ComputeMultipleInelasticStress::validParams();

  params.addParam<MaterialPropertyName>(
      "strain_energy_density",
      "psie",
      "Name of the strain energy density computed by this material model");
  params.addParam<MaterialPropertyName>(
      "interface_energy_density",
      "psii",
      "Name of the interface energy density computed by this material model");
  params.addParam<MaterialPropertyName>(
      "in_plane_degradation_function", "gip", "The in-plane degradation function");
  params.addParam<MaterialPropertyName>(
      "out_of_plane_degradation_function", "gop", "The out-of-plane degradation function");
  return params;
}

PFFComputeMultipleInelasticStress::PFFComputeMultipleInelasticStress(
    const InputParameters & parameters)
  : ComputeMultipleInelasticStress(parameters),
    // The strain energy density
    _psie_name(_base_name + getParam<MaterialPropertyName>("strain_energy_density")),
    _psie_active(declareProperty<Real>(_psie_name + "_active")),

    // The interface energy density
    _psii_name(_base_name + getParam<MaterialPropertyName>("interface_energy_density")),
    _psii_active(declareProperty<Real>(_psii_name + "_active")),
    _psii_active_old(getMaterialPropertyOldByName<Real>(_psii_name + "_active")),

    // The degradation function
    _gip_name(_base_name + getParam<MaterialPropertyName>("in_plane_degradation_function")),
    _gip(getMaterialProperty<Real>(_gip_name)),
    _gop_name(_base_name + getParam<MaterialPropertyName>("out_of_plane_degradation_function")),
    _gop(getMaterialProperty<Real>(_gop_name)),

    I2(RankTwoTensor::initIdentity),
    I4(RankFourTensor::initIdentitySymmetricFour)
{
  // Compute helper fourth order tensors
  I4_ip = I4;
  for (unsigned int k = 0; k < 3; k++)
    for (unsigned int l = 0; l < 3; l++)
      I4_ip(0, 2, k, l) = I4_ip(1, 2, k, l) = I4_ip(2, 2, k, l) = I4_ip(2, 1, k, l) =
          I4_ip(2, 0, k, l) = 0;
  I4_op = I4 - I4_ip;
  I4_ip_vol = I2.outerProduct(I2);
  I4_op_vol(2, 2, 0, 2) = I4(2, 2, 0, 2);
  I4_op_vol(2, 2, 1, 2) = I4(2, 2, 1, 2);
  I4_op_vol(2, 2, 2, 2) = I4(2, 2, 2, 2);
  I4_op_vol(2, 2, 2, 1) = I4(2, 2, 2, 1);
  I4_op_vol(2, 2, 2, 0) = I4(2, 2, 2, 0);
}

void
PFFComputeMultipleInelasticStress::initQpStatefulProperties()
{
  ComputeMultipleInelasticStress::initQpStatefulProperties();
  _psii_active[_qp] = 0;
}

void
PFFComputeMultipleInelasticStress::computeQpStress()
{
  ComputeMultipleInelasticStress::computeQpStress();

  // Compute coordinate transformation
  RealVectorValue nt(-_q_point[_qp](0), -_q_point[_qp](1), 0);
  nt /= nt.norm();
  RealVectorValue nz(0, 0, 1);
  RealVectorValue n = nz.cross(nt);
  Real cost = nt * nz;
  Real sint = std::sqrt(1 - cost * cost);
  RankTwoTensor nxn, offn;
  nxn.vectorOuterProduct(n, n);
  offn.fillFromInputVector({0, -n(2), n(1), n(2), 0, -n(0), -n(1), n(0), 0});
  RankTwoTensor Q = I2 * cost;
  Q += nxn * (1 - cost) + offn * sint;
  RankFourTensor QQ = Q.mixedProductIkJl(Q);
  RankFourTensor QtQt = Q.transpose().mixedProductIkJl(Q.transpose());

  // Transform strain to the current coordinates
  RankTwoTensor strainp = Q * _elastic_strain[_qp] * Q.transpose();

  // Decompose strain
  RankTwoTensor strain_ip = strainp;
  strain_ip(2, 0) = strain_ip(2, 1) = strain_ip(2, 2) = strain_ip(1, 2) = strain_ip(0, 2) = 0;

  Real strain_ip_tr = strain_ip(0, 0) + strain_ip(1, 1);
  RankTwoTensor strain_ip_vol;
  strain_ip_vol(0, 0) = strain_ip_vol(1, 1) = strain_ip_tr / 2;
  RankTwoTensor strain_ip_dev = strain_ip - strain_ip_vol;
  RankTwoTensor strain_ip_active = strain_ip_tr > 0 ? strain_ip : strain_ip_dev;
  RankTwoTensor strain_ip_inactive = strain_ip - strain_ip_active;

  RankTwoTensor strain_op = strainp - strain_ip;
  Real strain_op_tr = strain_op(2, 2);
  RankTwoTensor strain_op_dev = strain_op;
  strain_op_dev(2, 2) = 0;
  RankTwoTensor strain_op_active = strain_op_tr > 0 ? strain_op : strain_op_dev;
  RankTwoTensor strain_op_inactive = strain_op - strain_op_active;

  // Compute stress
  _stress[_qp] = _elasticity_tensor[_qp] * (_gip[_qp] * strain_ip_active + strain_ip_inactive +
                                            _gop[_qp] * strain_op_active + strain_op_inactive);
  _stress[_qp] = Q.transpose() * _stress[_qp] * Q;

  // Compute driving energy
  _psie_active[_qp] =
      strain_ip_active.doubleContraction(_elasticity_tensor[_qp] * strain_ip_active);
  _psii_active[_qp] =
      strain_op_active.doubleContraction(_elasticity_tensor[_qp] * strain_op_active);
  _psii_active[_qp] = std::max(_psii_active[_qp], _psii_active_old[_qp]);

  // Update jacobian
  RankFourTensor Jp = QQ * _Jacobian_mult[_qp] * QtQt;
  RankFourTensor dstrain_ip_inactive_dstrain = (strain_ip_tr < 0 ? 1 : 0) * I4_ip_vol * I4_ip;
  RankFourTensor dstrain_ip_active_dstrain = I4_ip - dstrain_ip_inactive_dstrain;
  RankFourTensor dstrain_op_inactive_dstrain = (strain_op_tr < 0 ? 1 : 0) * I4_op_vol * I4_op;
  RankFourTensor dstrain_op_active_dstrain = I4_op - dstrain_op_inactive_dstrain;
  Jp = Jp * (_gip[_qp] * dstrain_ip_active_dstrain + dstrain_ip_inactive_dstrain +
             _gop[_qp] * dstrain_op_active_dstrain + dstrain_op_inactive_dstrain);
  _Jacobian_mult[_qp] = QtQt * Jp * QQ;
}
