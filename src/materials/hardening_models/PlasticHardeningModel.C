//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PlasticHardeningModel.h"

InputParameters
PlasticHardeningModel::validParams()
{
  InputParameters params = Material::validParams();
  params += BaseNameInterface::validParams();

  params.set<bool>("compute") = false;
  params.suppressParameter<bool>("compute");

  return params;
}

PlasticHardeningModel::PlasticHardeningModel(const InputParameters & parameters)
  : Material(parameters), BaseNameInterface(parameters)
{
}
