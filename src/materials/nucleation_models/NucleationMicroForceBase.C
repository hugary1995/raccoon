//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "InputParameters.h"
#include "NucleationMicroForceBase.h"

InputParameters
NucleationMicroForceBase::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();

  // common parameters
  params.addParam<MaterialPropertyName>(
      "fracture_toughness", "Gc", "energy release rate or fracture toughness");
  params.addParam<MaterialPropertyName>(
      "normalization_constant", "c0", "The normalization constant $c_0$");
  params.addParam<MaterialPropertyName>(
      "regularization_length", "l", "the phase field regularization length");
  params.addParam<MaterialPropertyName>("lambda", "lambda", "Lame's first parameter lambda");
  params.addParam<MaterialPropertyName>("shear_modulus", "G", "shear modulus mu or G");
  params.addParam<MaterialPropertyName>(
      "stress_balance_name", "stress_balance", "Name of the stress balance function $F(\\sigma)$");
  params.addParam<MaterialPropertyName>("stress_name", "stress", "Name of the stress tensor");
  params.addParam<MaterialPropertyName>(
      "external_driving_force_name",
      "ex_driving",
      "Name of the material that holds the external_driving_force");
  params.addRequiredCoupledVar("phase_field", "Name of the phase-field (damage) variable");
  params.addParam<MaterialPropertyName>("degradation_function", "g", "The degradation function");
  return params;
}

NucleationMicroForceBase::NucleationMicroForceBase(const InputParameters & parameters)
  : Material(parameters),
    BaseNameInterface(parameters),
    _Gc(getADMaterialProperty<Real>(prependBaseName("fracture_toughness", true))),
    _c0(getADMaterialProperty<Real>(prependBaseName("normalization_constant", true))),
    _L(getADMaterialProperty<Real>(prependBaseName("regularization_length", true))),
    _lambda(getADMaterialProperty<Real>(prependBaseName("lambda", true))),
    _mu(getADMaterialProperty<Real>(prependBaseName("shear_modulus", true))),
    _stress(getADMaterialProperty<RankTwoTensor>(prependBaseName("stress_name", true))),
    _stress_balance(declareADProperty<Real>(prependBaseName("stress_balance_name", true))),
    _ex_driving(declareADProperty<Real>(prependBaseName("external_driving_force_name", true))),
    _d_name(getVar("phase_field", 0)->name()),
    _g_name(prependBaseName("degradation_function", true)),
    _g(getADMaterialProperty<Real>(_g_name)),
    _dg_dd(getADMaterialProperty<Real>(derivativePropertyName(_g_name, {_d_name})))
{
}
