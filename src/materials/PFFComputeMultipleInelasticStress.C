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
      "out_of_plane_fracture_toughness", "gamma", "The out-of-plane fracture toughness");
  params.addRequiredParam<Real>("out_of_plane_critical_fracture_energy",
                                "The out-of-plane critical fracture energy");
  return params;
}

PFFComputeMultipleInelasticStress::PFFComputeMultipleInelasticStress(
    const InputParameters & parameters)
  : ComputeMultipleInelasticStress(parameters),
    // The strain energy density
    _psie_name(_base_name + getParam<MaterialPropertyName>("strain_energy_density")),
    _psie(declareProperty<Real>(_psie_name)),
    _psie_active(declareProperty<Real>(_psie_name + "_active")),

    // The interface energy density
    _psii_name(_base_name + getParam<MaterialPropertyName>("interface_energy_density")),
    _psii(declareProperty<Real>(_psii_name)),
    _psii_active(declareProperty<Real>(_psii_name + "_active")),
    _psii_active_old(getMaterialPropertyOldByName<Real>(_psii_name + "_active")),

    // The degradation function
    _gip_name(_base_name + getParam<MaterialPropertyName>("in_plane_degradation_function")),
    _gip(getMaterialProperty<Real>(_gip_name)),

    // Out-of-plane fracture properties
    _c(declareProperty<Real>(_base_name + "debonding_indicator")),
    _c_old(getMaterialPropertyOldByName<Real>(_base_name + "debonding_indicator")),
    _gamma(getMaterialProperty<Real>(
        _base_name + getParam<MaterialPropertyName>("out_of_plane_fracture_toughness"))),
    _psic(getParam<Real>("out_of_plane_critical_fracture_energy")),

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
  _c[_qp] = 0;
}

Real
PFFComputeMultipleInelasticStress::g(const Real c, const unsigned int order)
{
  Real m = _gamma[_qp] / _psic;
  Real value = 0;

  Real num = (1 - c) * (1 - c);
  Real denom = num + m * c * (1 - 0.5 * c);

  Real eta = 1e-3;

  if (order == 0)
    value = num / denom;
  else if (order > 0)
  {
    Real dnum = -2 * (1 - c);
    Real ddenom = dnum + m * (1 - c);
    if (order == 1)
      value = (dnum * denom - num * ddenom) / denom / denom;
    else if (order == 2)
    {
      Real d2num = 2;
      Real d2denom = d2num - m;
      value = d2num / denom - 2 * dnum * ddenom / denom / denom - num * d2denom / denom / denom +
              2 * num * ddenom * ddenom / denom / denom / denom;
    }
  }

  return value * (1 - eta) + eta;
}

void
PFFComputeMultipleInelasticStress::computeQpStress()
{
  ComputeMultipleInelasticStress::computeQpStress();

  // Compute out of plane damage
  _c[_qp] = _c_old[_qp];
  Real residual = -_gamma[_qp] - g(_c[_qp], 1) * _psii_active_old[_qp];
  Real residual0 = residual;
  int its = 0;
  if (residual > 0)
    while (abs(residual) > 1e-8 && abs(residual) > 1e-6 * residual0)
    {
      Real jacobian = -g(_c[_qp], 2) * _psii_active_old[_qp];
      Real step = -residual / jacobian;
      _c[_qp] += step;
      residual = -_gamma[_qp] - g(_c[_qp], 1) * _psii_active_old[_qp];
      its++;
      if (its > 20)
        mooseError("Residual has dropped from ", residual0, " to ", residual);
    }
  _c[_qp] = std::min(_c[_qp], 1.0);

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
                                            g(_c[_qp], 0) * strain_op_active + strain_op_inactive);

  // Compute driving energy
  _psie[_qp] = _stress[_qp].doubleContraction(strain_ip) / 2;
  _psie_active[_qp] =
      strain_ip_active.doubleContraction(_elasticity_tensor[_qp] * strain_ip_active);
  _psii[_qp] = _stress[_qp].doubleContraction(strain_op) / 2;
  _psii_active[_qp] =
      strain_op_active.doubleContraction(_elasticity_tensor[_qp] * strain_op_active);

  // Transform stress back to the global coordinates
  _stress[_qp] = Q.transpose() * _stress[_qp] * Q;

  // Update jacobian
  RankFourTensor Jp = QQ * _Jacobian_mult[_qp] * QtQt;
  RankFourTensor dstrain_ip_inactive_dstrain = (strain_ip_tr < 0 ? 1 : 0) * I4_ip_vol * I4_ip;
  RankFourTensor dstrain_ip_active_dstrain = I4_ip - dstrain_ip_inactive_dstrain;
  RankFourTensor dstrain_op_inactive_dstrain = (strain_op_tr < 0 ? 1 : 0) * I4_op_vol * I4_op;
  RankFourTensor dstrain_op_active_dstrain = I4_op - dstrain_op_inactive_dstrain;
  Jp = Jp * (_gip[_qp] * dstrain_ip_active_dstrain + dstrain_ip_inactive_dstrain +
             g(_c[_qp], 0) * dstrain_op_active_dstrain + dstrain_op_inactive_dstrain);
  _Jacobian_mult[_qp] = QtQt * Jp * QQ;
}
