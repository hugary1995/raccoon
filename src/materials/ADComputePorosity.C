//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ADComputePorosity.h"
#include "RankTwoTensor.h"

registerMooseObject("raccoonApp", ADComputePorosity);

InputParameters
ADComputePorosity::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription(
      "compute porosity in poroelastic media as a function of fluid pressure and solid volumetric strain");
  params.addRequiredCoupledVar("porepressure", "fluid phase porepressure");
  params.addParam<MaterialPropertyName>(
      "porosity_name", "porosity", "Name of porosity material property");
  params.addRequiredRangeCheckedParam<Real>(
      "initial_porosity", "initial_porosity>0.0 & initial_porosity<1.0", "Initial porosity");
  params.addParam<MaterialPropertyName>("biot_coefficient", "biot_coefficient", "biot_coefficient");
  params.addParam<MaterialPropertyName>("biot_modulus", "biot_modulus", "biot_modulus");
  return params;
}

ADComputePorosity::ADComputePorosity(const InputParameters & parameters)
  : Material(parameters),
    _porepressure(adCoupledValue("porepressure")),
    _strain(getADMaterialPropertyByName<RankTwoTensor>("total_strain")),
    _porosity(declareADProperty<Real>(getParam<MaterialPropertyName>("porosity_name"))),
    _porosity_old(getMaterialPropertyOld<Real>(getParam<MaterialPropertyName>("porosity_name"))),
    _initial_porosity(getParam<Real>("initial_porosity")),
    _alpha(getADMaterialProperty<Real>("biot_coefficient")),
    _M(getADMaterialProperty<Real>("biot_modulus"))
{
}

void
ADComputePorosity::initQpStatefulProperties()
{
  _porosity[_qp] = _initial_porosity;
}

void
ADComputePorosity::computeQpProperties()
{
  _porosity[_qp] =
      _porepressure[_qp] / _M[_qp] + _alpha[_qp] * (_strain[_qp].trace()) + _initial_porosity;
}
