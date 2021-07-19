//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "GeneralizedExternalDrivingForce.h"
#include "Function.h"

registerADMooseObject("raccoonApp", GeneralizedExternalDrivingForce);

InputParameters
GeneralizedExternalDrivingForce::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params.addClassDescription("computes the Kumar external driving force","Ce()");
  params.addRequiredParam<Real>(
      "energy_release_rate", "energy release rate or fracture toughness");
  params.addRequiredParam<Real>(
      "phase_field_regularization_length", "phase_field_regularization length");
  params.addRequiredParam<Real>(
      "Lame_first_parameter", "Lame's first parameter Lambda");
  params.addRequiredParam<Real>(
      "shear_modulus", "shear modulus mu or G");
  params.addRequiredParam<Real>(
      "tensile_strength", "critical tensile strength");
  params.addRequiredParam<Real>(
      "compressive_strength", "critical compressive strength");
  params.addRequiredParam<Real>(
      "delta", "regularizaion length dependent parameter");
  params.addRequiredParam<MaterialPropertyName>(
      "rank_two_tensor","name of the stress tensor");
  params.addParam<MaterialPropertyName>(
      "external_driving_force_name", "ex_driving", "name of the material that holds the external_driving_force");
  return params;
}

GeneralizedExternalDrivingForce::GeneralizedExternalDrivingForce(const InputParameters & parameters)
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
    ,_rank_two_tensor(getParam<MaterialPropertyName>("rank_two_tensor"))
    ,_stress(getADMaterialProperty<RankTwoTensor>(_rank_two_tensor))
    ,_F_surface(declareADProperty<Real>("F_surface"))
    ,_J2(declareADProperty<Real>("J2"))
{
  _gamma_0 = _sigma_ts/6.0/(3.0*_Lambda+2.0*_mu)+_sigma_ts/6.0/_mu;
  _gamma_1 = (1.0+_delta)*(_sigma_cs-_sigma_ts)/(2.0*_sigma_ts*_sigma_cs);
  _gamma_2 = (1.0+_delta)*(_sigma_cs+_sigma_ts)/(2.0*_sigma_ts*_sigma_cs)*std::sqrt(3.0);
}

void
GeneralizedExternalDrivingForce::computeQpProperties()
{
  ADReal _temp = _Gc * 3.0/_L/8.0;
  ADReal I1 = _stress[_qp](0,0)+_stress[_qp](1,1)+_stress[_qp](2,2);
  ADReal I2 = _stress[_qp](0,0)*_stress[_qp](1,1)+_stress[_qp](0,0)*_stress[_qp](2,2)+_stress[_qp](1,1)*_stress[_qp](2,2)-pow(_stress[_qp](0,1),2)-pow(_stress[_qp](1,2),2)-pow(_stress[_qp](2,0),2);
  _J2[_qp] = ( pow(_stress[_qp](0,0)-_stress[_qp](1,1),2)+pow(_stress[_qp](1,1)-_stress[_qp](2,2),2)+pow(_stress[_qp](0,0)-_stress[_qp](2,2),2) )/6.0+pow(_stress[_qp](0,1),2)+pow(_stress[_qp](0,2),2)+pow(_stress[_qp](2,1),2);
  try{
    if (_J2[_qp]<0)
    {throw 11;}
  }
  catch(int e){
    std::cout<<"Negative J2 "<<_J2[_qp]<<std::endl;
    exit(e);
  }
  _beta_0[_qp]= _delta*_temp;
  _beta_1[_qp] = -_gamma_1*_temp+_gamma_0;
  _beta_2[_qp] = -_gamma_2*_temp+std::sqrt(3.0)*_gamma_0;
  _beta_3[_qp] = (1-std::sqrt(I1*I1)/I1)*(_J2[_qp]/2.0/_mu+I1*I1/6.0/(3.0*_Lambda+2.0*_mu));

  _ex_driving[_qp] = _beta_2[_qp]*std::sqrt(_J2[_qp])+_beta_1[_qp]*I1+_beta_0[_qp]+_beta_3[_qp];
  Real K = _Lambda+2.0*_mu/3.0;
  _F_surface[_qp] = _J2[_qp]/_mu + I1*I1/9.0/K - _ex_driving[_qp] - _temp;

}
