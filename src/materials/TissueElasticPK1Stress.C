//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "TissueElasticPK1Stress.h"

registerADMooseObject("raccoonApp", TissueElasticPK1Stress);

InputParameters
TissueElasticPK1Stress::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params.addClassDescription("Compute stress using the Mooney-Rivlin hyperelastic model");
  params.addParam<std::string>("base_name",
                               "Optional parameter that allows the user to define "
                               "multiple mechanics material systems on the same "
                               "block, i.e. for multiple phases");
  params.addParam<MaterialPropertyName>("k1", "k1", "k_1 in the tissue term");
  params.addParam<MaterialPropertyName>("k2", "k2", "k_2 in the tissue term");
  params.addParam<MaterialPropertyName>("M", "M", "orientation tensor in the tissue term");
  return params;
}

TissueElasticPK1Stress::TissueElasticPK1Stress(const InputParameters & parameters)
  : ADMaterial(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _F(getADMaterialPropertyByName<RankTwoTensor>("deformation_gradient")),
    _C(getADMaterialPropertyByName<RankTwoTensor>("mechanical_strain")),
    _k1(getMaterialProperty<Real>("k1")),
    _k2(getMaterialProperty<Real>("k2")),
    _M(getMaterialProperty<RankTwoTensor>("M")),
    _stress(declareADProperty<RankTwoTensor>(_base_name + "stress")),
    _cauchy_stress(declareADProperty<RankTwoTensor>(_base_name + "cauchy_stress"))
{
}

void
TissueElasticPK1Stress::computeQpProperties()
{
  ADReal J4 = _C[_qp].doubleContraction(_M[_qp]);
  ADReal T = smoothRamp(J4 - 1);
  ADRankTwoTensor dT_dC = dSmoothRamp(J4 - 1) * _M[_qp];

  ADRankTwoTensor S = 2 * _k1[_qp] * std::exp(_k2[_qp] * T * T) * dT_dC;

  _stress[_qp] = _F[_qp] * S;

  _cauchy_stress[_qp] = _stress[_qp] * _F[_qp].transpose() / _F[_qp].det();
}

ADReal
TissueElasticPK1Stress::smoothRamp(ADReal x, Real /*s*/)
{
  return x / (1 + std::exp(-x));
}

ADReal
TissueElasticPK1Stress::dSmoothRamp(ADReal x, Real /*s*/)
{
  ADReal ex = std::exp(x);
  return ex * (x + ex + 1) / (ex + 1) / (ex + 1);
}
