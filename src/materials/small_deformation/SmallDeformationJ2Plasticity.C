//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SmallDeformationJ2Plasticity.h"

InputParameters
SmallDeformationJ2Plasticity::validParams()
{
  InputParameters params = SmallDeformationPlasticityModel::validParams();

  return params;
}

SmallDeformationJ2Plasticity::SmallDeformationJ2Plasticity(const InputParameters & parameters)
  : SmallDeformationPlasticityModel(parameters)
{
}

void
SmallDeformationJ2Plasticity::updateState(ADRankTwoTensor & stress,
                                          ADRankTwoTensor & elastic_strain)
{
}
