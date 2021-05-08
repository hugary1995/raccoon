//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ElasticEnergyDensity.h"
#include "RankTwoTensor.h"

registerMooseObject("raccoonApp", ElasticEnergyDensity);

InputParameters
ElasticEnergyDensity::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params.addClassDescription(
      "computes the elastic energy density $\\frac{1}{2} \\stress : \\strain$");
  params.addParam<std::string>("base_name", "base name for stress and strain");
  params.addParam<MaterialPropertyName>(
      "elastic_energy_name", "E_el", "name of the elastic energy density material");

  return params;
}

ElasticEnergyDensity::ElasticEnergyDensity(const InputParameters & parameters)
  : ADMaterial(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _E_el(declareADProperty<Real>(_base_name +
                                  getParam<MaterialPropertyName>("elastic_energy_name"))),
    _stress(getADMaterialPropertyByName<RankTwoTensor>(_base_name + "stress")),
    _strain(getADMaterialPropertyByName<RankTwoTensor>(_base_name + "mechanical_strain"))
{
}

void
ElasticEnergyDensity::computeQpProperties()
{
  _E_el[_qp] = 0.5 * _stress[_qp].doubleContraction(_strain[_qp]);
}
