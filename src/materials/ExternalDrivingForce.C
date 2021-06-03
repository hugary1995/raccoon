//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ExternalDrivingForce.h"
#include "Function.h"

registerADMooseObject("raccoonApp", ExternalDrivingForce);

InputParameters
ExternalDrivingForce::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params.addClassDescription("computes the Kumar coeffs"
                             "Ce()");
//  params.addRequiredParam<Real>(
//      "coef", "The coefficient describing the mismatch between the film and the substrate");
//  params.addRequiredCoupledVar(
//      "displacements",
//      "The displacements appropriate for the simulation geometry and coordinate system");
  params.addParam<Real>(
      "energy_release_rate", 1, "energy release rate or fracture toughness");
  params.addParam<Real>(
      "phase_field_regularization_length", 1, "phase_field_regularization length");
  params.addParam<Real>(
      "tensile_strength", 1, "tensile strength");
  params.addParam<Real>(
      "compressive_strength", 1, "compressive strength");
  params.addParam<Real>(
      "delta", 1, "delta");
  params.addParam<Real>(
      "alpha_L", 1, "alpha_L");
  params.addParam<MaterialPropertyName>(
      "external_driving_force_name", "ex_driving", "name of the material that holds the external_driving_force");
  // params.addParam<MaterialPropertyName>(
  //     "beta_0","beta_0","the zeroth beta coefficeient"  )
//  params.addParam<MaterialPropertyName>(
//      "first_invariant", "invar_1", "The first standard invariants of stress tensor");
//  params.addParam<MaterialPropertyName>(
//      "second_invariant", "invar_2", "The second standard invariants of stress tensor");
params.addCoupledVar("first_invariant","The first standard invariants of stress tensor");
params.addCoupledVar("second_invariant","The second standard invariants of stress tensor");

  return params;
}

ExternalDrivingForce::ExternalDrivingForce(const InputParameters & parameters)
  : ADMaterial(parameters),
    _ex_driving_name(getParam<MaterialPropertyName>("external_driving_force_name")),
    _ex_driving(declareADProperty<Real>(_ex_driving_name)),
    // _Gc(getADMaterialProperty<Real>("energy_release_rate")),
    _Gc(getADMaterialProperty<Real>("Gc")),
    _L(getADMaterialProperty<Real>("l")),
    _sigma_ts(getParam<Real>("tensile_strength")),
    _sigma_cs(getParam<Real>("compressive_strength")),
    _delta(getParam<Real>("delta")),
    _alpha_L(getParam<Real>("alpha_L"))
    ,_beta_0(declareADProperty<Real>("beta_0"))
    ,_beta_1(declareADProperty<Real>("beta_1"))
    ,_beta_2(declareADProperty<Real>("beta_2"))
    ,_beta_3(declareADProperty<Real>("beta_3"))
    ,_invar_1(adCoupledValue("first_invariant"))
    ,_invar_2(adCoupledValue("second_invariant"))
//    _invar_1(getParam<Real>(getParam<MaterialPropertyName>("first_invariant"))),
//    _invar_2(getParam<Real>(getParam<MaterialPropertyName>("second_invariant")))

//    _E_int(declareADProperty<Real>(_E_int_name)),
 //   _coef(getParam<Real>("coef")),
  //  _ndisp(coupledComponents("displacements")),
//    _disp(3)
{
  _gamma_0 = -2*_sigma_ts*_sigma_cs/(_sigma_cs+_sigma_ts)/std::sqrt(3);
  _gamma_1 = (_sigma_cs-_sigma_ts)/(_sigma_cs+_sigma_ts)/std::sqrt(3);

  // _beta_0 = _delta*_temp;
  // _beta_1 = (1+_delta)*_gamma_1/_gamma_0*_temp;
  // _beta_2 = (1+_delta)/_gamma_0*_temp;
  // _beta_3 = _alpha_L*_L;
}

void
ExternalDrivingForce::computeQpProperties()
{
  ADReal _temp = _Gc[_qp] * 3.0/_L[_qp]/8.0;
  ADReal J2 = _invar_1[_qp]*_invar_1[_qp]/3.0-_invar_2[_qp];
  _beta_0[_qp]= _delta*_temp;
  _beta_1[_qp] = (1+_delta)*_gamma_1/_gamma_0*_temp;
  _beta_2[_qp] = (1+_delta)/_gamma_0*_temp;
  _beta_3[_qp] = _alpha_L*_L[_qp];
   _ex_driving[_qp] = (_beta_2[_qp]*std::sqrt(J2)+_beta_1[_qp]*_invar_1[_qp]+_beta_0[_qp])/(1.0+_beta_3[_qp]*_invar_1[_qp]*_invar_1[_qp]);
   // _ex_driving[_qp] = std::sqrt(J2);
  // _ex_driving[_qp] = ce*4.0/3.0-_Gc/2.0/_L;
//  ADRealVectorValue u((*_disp[0])[_qp], (*_disp[1])[_qp], (*_disp[2])[_qp]);
  //_E_int[_qp] = 0.5 * _coef * u * u;
}
