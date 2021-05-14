//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PlasticHardeningModel.h"

InputParameters
PlasticHardeningModel::validParams()
{
  InputParameters params = Material::validParams();
  params.addParam<std::string>("base_name",
                               "Optional parameter that allows the user to define "
                               "multiple mechanics material systems on the same "
                               "block, i.e. for multiple phases");

  params.set<bool>("compute") = false;
  params.suppressParameter<bool>("compute");

  return params;
}

PlasticHardeningModel::PlasticHardeningModel(const InputParameters & parameters)
  : Material(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : "")
{
}