//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "GeneralizedExternalDrivingForcefracold.h"
#include "Function.h"

registerADMooseObject("raccoonApp", GeneralizedExternalDrivingForcefracold);

InputParameters
GeneralizedExternalDrivingForcefracold::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params.addClassDescription("computes the Kumar coeffs"
                             "Ce()");
  params.addRequiredParam<Real>(
      "energy_release_rate", "energy release rate or fracture toughness");
  params.addRequiredParam<Real>(
      "phase_field_regularization_length", "phase_field_regularization length");
  params.addRequiredParam<Real>(
      "Lame_first_parameter", "Lame's first parameter Lambda");
  params.addRequiredParam<Real>(
      "shear_modulus", "shear modulus mu or G");
  params.addRequiredParam<Real>(
      "tensile_strength", "tensile strength");
  params.addRequiredParam<Real>(
      "compressive_strength", "compressive strength");
  params.addRequiredParam<Real>(
      "delta", "delta");

  params.addParam<MaterialPropertyName>(
      "external_driving_force_name", "ex_driving", "name of the material that holds the external_driving_force");
params.addCoupledVar("stress_00","00");
params.addCoupledVar("stress_01","01");
params.addCoupledVar("stress_02","02");
params.addCoupledVar("stress_11","11");
params.addCoupledVar("stress_12","12");
params.addCoupledVar("stress_22","22");
  return params;
}

GeneralizedExternalDrivingForcefracold::GeneralizedExternalDrivingForcefracold(const InputParameters & parameters)
  : ADMaterial(parameters),
    _ex_driving_name(getParam<MaterialPropertyName>("external_driving_force_name")),
    _ex_driving(declareADProperty<Real>(_ex_driving_name)),
    _Gc(getParam<Real>("energy_release_rate")),
    _L(getParam<Real>("phase_field_regularization_length")),
    _Lambda(getParam<Real>("Lame_first_parameter")),
    _mu(getParam<Real>("shear_modulus")),
    _sigma_ts(getParam<Real>("tensile_strength")),
    _sigma_cs(getParam<Real>("compressive_strength")),
    _delta(getParam<Real>("delta"))
    ,_beta_0(declareADProperty<Real>("beta_0"))
    ,_beta_1(declareADProperty<Real>("beta_1"))
    ,_beta_2(declareADProperty<Real>("beta_2"))
    ,_beta_3(declareADProperty<Real>("beta_3"))
    ,_stress_00(adCoupledValue("stress_00"))
    ,_stress_01(adCoupledValue("stress_01"))
    ,_stress_02(adCoupledValue("stress_02"))
    ,_stress_11(adCoupledValue("stress_11"))
    ,_stress_12(adCoupledValue("stress_12"))
    ,_stress_22(adCoupledValue("stress_22"))
    ,_F_surface(declareADProperty<Real>("F_surface"))
    ,_J2(declareADProperty<Real>("J2"))
{
  _gamma_0 = _sigma_ts/6.0/(3.0*_Lambda+2.0*_mu)+_sigma_ts/6.0/_mu;
  _gamma_1 = (1.0+_delta)*(_sigma_cs-_sigma_ts)/(2.0*_sigma_ts*_sigma_cs);
  _gamma_2 = (1.0+_delta)*(_sigma_cs+_sigma_ts)/(2.0*_sigma_ts*_sigma_cs)*std::sqrt(3.0);
}

void
GeneralizedExternalDrivingForcefracold::computeQpProperties()
{
  ADReal _temp = _Gc * 3.0/_L/8.0;
  ADRankTwoTensor _stress;
  _stress(0,0) = _stress_00[_qp];
  _stress(0,1) = _stress_01[_qp];
  _stress(0,2) = _stress_02[_qp];
  _stress(1,1) = _stress_11[_qp];
  _stress(1,2) = _stress_12[_qp];
  _stress(2,2) = _stress_22[_qp];

  ADReal I1 = _stress(0,0)+_stress(1,1)+_stress(2,2);
  ADReal I2 = _stress(0,0)*_stress(1,1)+_stress(0,0)*_stress(2,2)+_stress(1,1)*_stress(2,2)-pow(_stress(0,1),2)-pow(_stress(1,2),2)-pow(_stress(2,0),2);
  // _J2[_qp] = ( pow(_stress(0,0)-_stress(1,1),2)+pow(_stress(1,1)-_stress(2,2),2)+pow(_stress(0,0)-_stress(2,2),2) )/6.0+pow(_stress(0,1),2)+pow(_stress(0,2),2)+pow(_stress(2,1),2);
  _J2[_qp] = I1*I1/3-I2;
  // ADReal I1 = _stress[_qp](0,0)+_stress[_qp](1,1)+_stress[_qp](2,2);
  // ADReal I2 = _stress[_qp](0,0)*_stress[_qp](1,1)+_stress[_qp](0,0)*_stress[_qp](2,2)+_stress[_qp](1,1)*_stress[_qp](2,2)-pow(_stress[_qp](0,1),2)-pow(_stress[_qp](1,2),2)-pow(_stress[_qp](2,0),2);
  // _J2[_qp] = ( pow(_stress[_qp](0,0)-_stress[_qp](1,1),2)+pow(_stress[_qp](1,1)-_stress[_qp](2,2),2)+pow(_stress[_qp](0,0)-_stress[_qp](2,2),2) )/6.0+pow(_stress[_qp](0,1),2)+pow(_stress[_qp](0,2),2)+pow(_stress[_qp](2,1),2);
  // ADReal I1= _invar_1[_qp];
  // ADReal I2 = _invar_2[_qp];
  // _J2[_qp] = I1*I1/3.0-I2;
  _beta_0[_qp]= _delta*_temp;
  _beta_1[_qp] = -_gamma_1*_temp+_gamma_0;
  _beta_2[_qp] = -_gamma_2*_temp+std::sqrt(3.0)*_gamma_0;
  _beta_3[_qp] = (1-std::sqrt(I1*I1)/I1)*(_J2[_qp]/2.0/_mu+I1*I1/6.0/(3.0*_Lambda+2.0*_mu));

  _ex_driving[_qp] = _beta_2[_qp]*std::sqrt(_J2[_qp])+_beta_1[_qp]*I1+_beta_0[_qp]+_beta_3[_qp];
  Real K = _Lambda+2.0*_mu/3.0;
  _F_surface[_qp] = _J2[_qp]/_mu + I1*I1/9.0/K - _ex_driving[_qp] - _temp;
}
