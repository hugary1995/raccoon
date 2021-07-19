//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "GeneralizedExternalDrivingForceold.h"
#include "Function.h"
#include <iostream>
#include <fstream>
#include <time.h>

registerADMooseObject("raccoonApp", GeneralizedExternalDrivingForceold);

InputParameters
GeneralizedExternalDrivingForceold::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params.addClassDescription("computes the Kumar coeffs"
                             "Ce()");
//  params.addRequiredParam<Real>(
//      "coef", "The coefficient describing the mismatch between the film and the substrate");
//  params.addRequiredCoupledVar(
//      "displacements",
//      "The displacements appropriate for the simulation geometry and coordinate system");
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

GeneralizedExternalDrivingForceold::GeneralizedExternalDrivingForceold(const InputParameters & parameters)
  : ADMaterial(parameters),
    _ex_driving_name(getParam<MaterialPropertyName>("external_driving_force_name")),
    _ex_driving(declareADProperty<Real>(_ex_driving_name)),
    // _Gc(getADMaterialProperty<Real>("energy_release_rate")),
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
    ,_invar_1(adCoupledValue("first_invariant"))
    ,_invar_2(adCoupledValue("second_invariant"))
    ,_F_surface(declareADProperty<Real>("F_surface"))
    ,_J2(declareADProperty<Real>("J2"))
//    _invar_1(getParam<Real>(getParam<MaterialPropertyName>("first_invariant"))),
//    _invar_2(getParam<Real>(getParam<MaterialPropertyName>("second_invariant")))

//    _E_int(declareADProperty<Real>(_E_int_name)),
 //   _coef(getParam<Real>("coef")),
  //  _ndisp(coupledComponents("displacements")),
//    _disp(3)
{
  _gamma_0 = _sigma_ts/6.0/(3.0*_Lambda+2.0*_mu)+_sigma_ts/6.0/_mu;
  _gamma_1 = (1.0+_delta)*(_sigma_cs-_sigma_ts)/(2.0*_sigma_ts*_sigma_cs);
  _gamma_2 = (1.0+_delta)*(_sigma_cs+_sigma_ts)/(2.0*_sigma_ts*_sigma_cs)*std::sqrt(3.0);
}

void
GeneralizedExternalDrivingForceold::computeQpProperties()
{
  ADReal _temp = _Gc * 3.0/_L/8.0;
  ADReal I1= _invar_1[_qp];
  ADReal I2 = _invar_2[_qp];
  _J2[_qp] = I1*I1/3.0-I2;
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
  // if (_J2[_qp]<0.0001132)
  //
  //   // time_t timer;
  //   // time(&timer);
  //   std::ofstream myfile;
  //   myfile.open ("nan.csv",std::ofstream::app);
  //   // myfile << "_invar_1[_qp],_invar_2[_qp],_beta_0[_qp],_beta_1[_qp],_beta_2[_qp],_beta_3[_qp],_J2[_qp],_ex_driving[_qp],\n ";
  //   myfile <<  _invar_1[_qp] <<","<< _invar_2[_qp]<<","<< _beta_0[_qp] <<","<< _beta_1[_qp]<<","<< _beta_2[_qp] <<","<< _beta_3[_qp] <<","<< _J2[_qp] <<","<< _ex_driving[_qp]<<","<<_q_point[_qp] <<",\n";
  //   myfile.close();
  //   // std::cout << "_invar_1[_qp] = " << _invar_1[_qp] << std::endl;
  //   // std::cout << "_invar_2[_qp] = " << _invar_2[_qp] << std::endl;
  //   // std::cout << "_beta_0[_qp] = " << _beta_0[_qp] << std::endl;
  //   // std::cout << "_beta_1[_qp] = " << _beta_1[_qp] << std::endl;
  //   // std::cout << "_beta_2[_qp] = " << _beta_2[_qp] << std::endl;
  //   // std::cout << "_beta_3[_qp] = " << _beta_3[_qp] << std::endl;
  //   // std::cout << "_J2[_qp] = " << _J2[_qp] << std::endl;
  //   // std::cout << "_ex_driving[_qp] = " << _ex_driving[_qp] << std::endl;
  // }
  // else
  // {
  //   std::ofstream myfile2;
  //   myfile2.open ("normal.csv",std::ofstream::app);
  //   // myfile << "_invar_1[_qp],_invar_2[_qp],_beta_0[_qp],_beta_1[_qp],_beta_2[_qp],_beta_3[_qp],_J2[_qp],_ex_driving[_qp],\n ";
  //   myfile2 <<  _invar_1[_qp] <<","<< _invar_2[_qp]<<","<< _beta_0[_qp] <<","<< _beta_1[_qp]<<","<< _beta_2[_qp] <<","<< _beta_3[_qp] <<","<< _J2[_qp] <<","<< _ex_driving[_qp] <<",\n";
  //   myfile2.close();
  // }

      // myfile << "This is the first cell in the first column.\n";
      // myfile << "a,b,c,\n";
      // myfile << "c,s,v,\n";
      // myfile << "1,2,3.456\n";
      // myfile << "semi;colon";
      // myfile.close();
}
