//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADMaterial.h"

class GeneralizedExternalDrivingForcefrac : public ADMaterial
{
public:
  static InputParameters validParams();

  GeneralizedExternalDrivingForcefrac(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  const MaterialPropertyName _ex_driving_name;

  ADMaterialProperty<Real> & _ex_driving;

  /// energy release rate
  //const GenericMaterialProperty<Real,true> & _Gc;
  const Real & _Gc;
  // const Real & _Gc;

  /// phase field regularization length
  const Real & _L;
  // const Real & _L;

  const Real & _Lambda;

  const Real & _mu;

  /// tensile strength
  const Real & _sigma_ts;

  /// compressive strength
  const Real & _sigma_cs;

  ///
  const Real & _delta;

  ///
  ADMaterialProperty<Real> & _beta_0;
  ADMaterialProperty<Real> & _beta_1;
  ADMaterialProperty<Real> & _beta_2;
  ADMaterialProperty<Real> & _beta_3;
  Real _gamma_0;
  Real _gamma_1;
  Real _gamma_2;
  // const ADVariableValue & _invar_1;
  // const ADVariableValue & _invar_2;
  // const ADMaterialProperty<Real> & _invar_1;
  // const ADMaterialProperty<Real> & _invar_2;
  // const MaterialPropertyName _rank_two_tensor;
  // const ADMaterialProperty<RankTwoTensor> & _stress;
  const ADVariableValue & _stress_00;
  const ADVariableValue & _stress_01;
  const ADVariableValue & _stress_02;
  const ADVariableValue & _stress_11;
  const ADVariableValue & _stress_12;
  const ADVariableValue & _stress_22;
  ADMaterialProperty<Real> & _F_surface;
  ADMaterialProperty<Real> & _J2;
};
