//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADSPFVolStrTimeDerivative.h"

registerMooseObject("raccoonApp", ADSPFVolStrTimeDerivative);

InputParameters
ADSPFVolStrTimeDerivative::validParams()
{
  InputParameters params = ADTimeKernel::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription("Single phase flow volumetric strain time derivative term.");
  params.addParam<MaterialPropertyName>("biot_coefficient", "alpha", "The Biot coefficient");
  return params;
}

ADSPFVolStrTimeDerivative::ADSPFVolStrTimeDerivative(const InputParameters & parameters)
  : ADTimeKernel(parameters),
    BaseNameInterface(parameters),
    _strain(getADMaterialPropertyByName<RankTwoTensor>(prependBaseName("total_strain"))),
    _strain_old(getMaterialPropertyOldByName<RankTwoTensor>(prependBaseName("total_strain"))),
    _alpha(getADMaterialProperty<Real>(prependBaseName("biot_coefficient", true)))
{
}

ADReal
ADSPFVolStrTimeDerivative::computeQpResidual()
{
  ADReal vol_strain_rate = (_strain[_qp].trace() - _strain_old[_qp].trace()) / _dt;
  return _test[_i][_qp] * _alpha[_qp] * vol_strain_rate;
}
