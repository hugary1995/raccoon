//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LargeDeformationNewtonianViscosity.h"

registerMooseObject("raccoonApp", LargeDeformationNewtonianViscosity);

InputParameters
LargeDeformationNewtonianViscosity::validParams()
{
  InputParameters params = LargeDeformationViscoelasticityModel::validParams();
  params.addClassDescription("Newtonian viscosity.");
  params.addParam<MaterialPropertyName>("viscosity", "viscosity", "Name of the viscosity material");
  params.addParam<MaterialPropertyName>("degradation_function", "g", "The degradation function");

  return params;
}

LargeDeformationNewtonianViscosity::LargeDeformationNewtonianViscosity(
    const InputParameters & parameters)
  : LargeDeformationViscoelasticityModel(parameters),
    _eta(getADMaterialProperty<Real>(_base_name + getParam<MaterialPropertyName>("viscosity"))),
    _g(getADMaterialProperty<Real>(_base_name +
                                   getParam<MaterialPropertyName>("degradation_function")))
{
}

ADRankTwoTensor
LargeDeformationNewtonianViscosity::computeCauchyStress(const ADRankTwoTensor & Fm,
                                                        const RankTwoTensor & Fm_old)
{
  ADRankTwoTensor Fm_dot = (Fm - Fm_old) / _dt;
  ADRankTwoTensor Fm_inv = Fm.inverse();
  ADRankTwoTensor D = (Fm_dot * Fm_inv + Fm_inv.transpose() * Fm_dot.transpose()) / 2;
  return _g[_qp] * _eta[_qp] * D.deviatoric();
}
