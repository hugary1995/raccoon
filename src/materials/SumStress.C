//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SumStress.h"

registerMooseObject("raccoonApp", SumStress);

InputParameters
SumStress::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params.addRequiredParam<std::vector<std::string>>("base_names",
                                                    "the base names of the stresses to be summed");
  params.addParam<std::string>("base_name", "the base name of the summed stress");
  params.addParam<std::string>("stress_name", "stress", "name of the stress");
  return params;
}

SumStress::SumStress(const InputParameters & parameters)
  : ADMaterial(parameters),
    _stress_name(getParam<std::string>("stress_name")),
    _base_names(getParam<std::vector<std::string>>("base_names")),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _stress(declareADProperty<RankTwoTensor>(_base_name + _stress_name)),
    _N(_base_names.size()),
    _stresses(_N)
{
  for (unsigned int i = 0; i < _N; i++)
    _stresses[i] = &getADMaterialPropertyByName<RankTwoTensor>(_base_names[i] + "_" + _stress_name);
}

void
SumStress::initQpStatefulProperties()
{
  computeQpProperties();
}

void
SumStress::computeQpProperties()
{
  _stress[_qp].zero();
  for (unsigned int i = 0; i < _N; i++)
    _stress[_qp] += (*_stresses[i])[_qp];
}
