//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "StressTriaxiality.h"

registerADMooseObject("raccoonApp", StressTriaxiality);

InputParameters
StressTriaxiality::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("Compute the deformation gradient.");
  params.addParam<std::string>("base_name",
                               "Optional parameter that allows the user to define "
                               "multiple mechanics material systems on the same "
                               "block, i.e. for multiple phases");
  return params;
}

StressTriaxiality::StressTriaxiality(const InputParameters & parameters)
  : Material(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _T(declareADProperty<Real>(_base_name + "stress_triaxiality")),
    _stress(getADMaterialProperty<RankTwoTensor>(_base_name + "stress"))
{
}

void
StressTriaxiality::computeQpProperties()
{
  ADRankTwoTensor s = _stress[_qp].deviatoric();
  ADReal s_norm = std::sqrt(1.5 * s.doubleContraction(s));
  _T[_qp] = _stress[_qp].trace() / 3 / s_norm;
}
