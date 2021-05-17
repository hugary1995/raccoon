//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LargeDeformationJIntegral.h"

registerMooseObject("raccoonApp", LargeDeformationJIntegral);

InputParameters
LargeDeformationJIntegral::validParams()
{
  InputParameters params = SideIntegralPostprocessor::validParams();
  params.addClassDescription(
      "This class computes the J integral for a phase-field model of fracture");
  params.addParam<std::string>("base_name",
                               "Optional parameter that allows the user to define "
                               "multiple mechanics material systems on the same "
                               "block, i.e. for multiple phases");
  params.addRequiredParam<RealVectorValue>("J_direction", "direction of J integral");
  params.addRequiredParam<MaterialPropertyName>("strain_energy_density",
                                                "The strain energy density");
  return params;
}

LargeDeformationJIntegral::LargeDeformationJIntegral(const InputParameters & parameters)
  : SideIntegralPostprocessor(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _stress(getADMaterialPropertyByName<RankTwoTensor>(_base_name + "stress")),
    _psie(getADMaterialPropertyByName<Real>(
        _base_name + getParam<MaterialPropertyName>("strain_energy_density"))),
    _Fm(getADMaterialPropertyByName<RankTwoTensor>(_base_name + "mechanical_deformation_gradient")),
    _t(getParam<RealVectorValue>("J_direction"))
{
}

Real
LargeDeformationJIntegral::computeQpIntegral()
{
  ADRankTwoTensor I2(ADRankTwoTensor::initIdentity);
  ADRankTwoTensor H = _Fm[_qp] - I2;
  ADRankTwoTensor PK1 = _Fm[_qp].det() * _stress[_qp] * _Fm[_qp].inverse().transpose();
  ADRankTwoTensor Sigma = _psie[_qp] * I2 - H.transpose() * PK1;
  return raw_value(_t * Sigma * _normals[_qp]);
}
