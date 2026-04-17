//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "PsicDegModel.h"

InputParameters
PsicDegModel::validParams()
{
  InputParameters params = Material::validParams();

  params += BaseNameInterface::validParams();

  params.set<bool>("compute") = false;
  params.suppressParameter<bool>("compute");

  return params;
}

PsicDegModel::PsicDegModel(const InputParameters & parameters)
  : Material(parameters),BaseNameInterface(parameters)
{
}
