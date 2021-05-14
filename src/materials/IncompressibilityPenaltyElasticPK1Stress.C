//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "IncompressibilityPenaltyElasticPK1Stress.h"

registerMooseObject("raccoonApp", IncompressibilityPenaltyElasticPK1Stress);

InputParameters
IncompressibilityPenaltyElasticPK1Stress::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params.addClassDescription("Compute stress to penalize volume change");
  params.addParam<std::string>("base_name",
                               "Optional parameter that allows the user to define "
                               "multiple mechanics material systems on the same "
                               "block, i.e. for multiple phases");
  params.addRequiredRangeCheckedParam<Real>(
      "epsilon1", "epsilon1 > 0", "epsilon_1 parameter in the penalty functional");
  params.addRequiredRangeCheckedParam<Real>(
      "epsilon2", "epsilon2 >= 1", "epsilon_2 parameter in the penalty functional");
  return params;
}

IncompressibilityPenaltyElasticPK1Stress::IncompressibilityPenaltyElasticPK1Stress(
    const InputParameters & parameters)
  : ADMaterial(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _F(getADMaterialPropertyByName<RankTwoTensor>("deformation_gradient")),
    _C(getADMaterialPropertyByName<RankTwoTensor>("mechanical_strain")),
    _epsilon1(getParam<Real>("epsilon1")),
    _epsilon2(getParam<Real>("epsilon2")),
    _stress(declareADProperty<RankTwoTensor>(_base_name + "stress")),
    _cauchy_stress(declareADProperty<RankTwoTensor>(_base_name + "cauchy_stress"))
{
}

void
IncompressibilityPenaltyElasticPK1Stress::computeQpProperties()
{
  ADRankTwoTensor Cinv = _C[_qp].inverse();
  ADReal detC = _C[_qp].det();

  ADRankTwoTensor S =
      2 * _epsilon1 * _epsilon2 * (std::pow(detC, _epsilon2) - std::pow(detC, -_epsilon2)) * Cinv;

  _stress[_qp] = _F[_qp] * S;

  _cauchy_stress[_qp] = _stress[_qp] * _F[_qp].transpose() / _F[_qp].det();
}
