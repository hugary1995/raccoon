//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "ElasticEnergyDensity.h"
#include "RankTwoTensor.h"

registerADMooseObject("raccoonApp", ElasticEnergyDensity);

defineADLegacyParams(ElasticEnergyDensity);

template <ComputeStage compute_stage>
InputParameters
ElasticEnergyDensity<compute_stage>::validParams()
{
  InputParameters params = ADMaterial<compute_stage>::validParams();
  params.addClassDescription("computes the elastic energy density $\\stress : \\strain$");
  params.addParam<std::string>("base_name", "base name for stress and strain");
  params.addParam<MaterialPropertyName>(
      "elastic_energy_name", "E_el", "name of the elastic energy density material");

  return params;
}

template <ComputeStage compute_stage>
ElasticEnergyDensity<compute_stage>::ElasticEnergyDensity(const InputParameters & parameters)
  : ADMaterial<compute_stage>(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _E_el(declareADProperty<Real>(_base_name +
                                  getParam<MaterialPropertyName>("elastic_energy_name"))),
    _stress(getADMaterialPropertyByName<RankTwoTensor>(_base_name + "stress")),
    _strain(getADMaterialPropertyByName<RankTwoTensor>(_base_name + "mechanical_strain"))
{
}

template <ComputeStage compute_stage>
void
ElasticEnergyDensity<compute_stage>::computeQpProperties()
{
  _E_el[_qp] = 0.5 * _stress[_qp].doubleContraction(_strain[_qp]);
}
