//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "MooneyRivlinElasticPK1Stress.h"

registerMooseObject("raccoonApp", MooneyRivlinElasticPK1Stress);

InputParameters
MooneyRivlinElasticPK1Stress::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params.addClassDescription("Compute stress using the Mooney-Rivlin hyperelastic model");
  params.addParam<std::string>("base_name",
                               "Optional parameter that allows the user to define "
                               "multiple mechanics material systems on the same "
                               "block, i.e. for multiple phases");
  params.addParam<MaterialPropertyName>("eta1", "eta1", "eta_1 in the Mooney-Rivlin model");
  params.addParam<MaterialPropertyName>("eta2", "eta2", "eta_2 in the Mooney-Rivlin model");
  params.addParam<MaterialPropertyName>("eta3", "eta3", "eta_3 in the Mooney-Rivlin model");
  return params;
}

MooneyRivlinElasticPK1Stress::MooneyRivlinElasticPK1Stress(const InputParameters & parameters)
  : ADMaterial(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _F(getADMaterialPropertyByName<RankTwoTensor>("deformation_gradient")),
    _C(getADMaterialPropertyByName<RankTwoTensor>("mechanical_strain")),
    _eta1(getMaterialProperty<Real>("eta1")),
    _eta2(getMaterialProperty<Real>("eta2")),
    _eta3(getMaterialProperty<Real>("eta3")),
    _stress(declareADProperty<RankTwoTensor>(_base_name + "stress")),
    _cauchy_stress(declareADProperty<RankTwoTensor>(_base_name + "cauchy_stress"))
{
}

void
MooneyRivlinElasticPK1Stress::computeQpProperties()
{
  Real delta = 2 * _eta1[_qp] + 4 * _eta2[_qp] + 2 * _eta3[_qp];
  ADRankTwoTensor I2(ADRankTwoTensor::initIdentity);
  ADRankTwoTensor Cinv = _C[_qp].inverse();

  ADRankTwoTensor S = 2 * _eta1[_qp] * I2 + 2 * _eta2[_qp] * (_C[_qp].trace() * I2 - _C[_qp]) +
                      2 * _eta3[_qp] * _C[_qp].det() * Cinv - delta * Cinv;

  _stress[_qp] = _F[_qp] * S;

  _cauchy_stress[_qp] = _stress[_qp] * _F[_qp].transpose() / _F[_qp].det();
}
