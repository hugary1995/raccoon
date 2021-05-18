//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LargeDeformationJIntegral.h"

registerMooseObject("raccoonApp", LargeDeformationJIntegral);

InputParameters
LargeDeformationJIntegral::validParams()
{
  InputParameters params = SideIntegralPostprocessor::validParams();
  params += BaseNameInterface::validParams();
  params.addClassDescription(
      "This class computes the J integral for a phase-field model of fracture");

  params.addRequiredParam<RealVectorValue>("J_direction", "direction of J integral");
  params.addRequiredParam<MaterialPropertyName>("strain_energy_density",
                                                "The strain energy density");
  return params;
}

LargeDeformationJIntegral::LargeDeformationJIntegral(const InputParameters & parameters)
  : SideIntegralPostprocessor(parameters),
    BaseNameInterface(parameters),
    _stress(getADMaterialPropertyByName<RankTwoTensor>(prependBaseName("stress"))),
    _psie(getADMaterialPropertyByName<Real>(prependBaseName("strain_energy_density", true))),
    _Fm(getADMaterialPropertyByName<RankTwoTensor>(
        prependBaseName("mechanical_deformation_gradient"))),
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
