//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "LDElasticEnergyDensityBase.h"

InputParameters
LDElasticEnergyDensityBase::validParams()
{
  InputParameters params = ElasticEnergyDensityBase::validParams();
  return params;
}

LDElasticEnergyDensityBase::LDElasticEnergyDensityBase(const InputParameters & params)
  : ElasticEnergyDensityBase(params),
    _Fm(getADMaterialProperty<RankTwoTensor>(prependBaseName("mechanical_deformation_gradient"))),
    _Fp(hasADMaterialProperty<RankTwoTensor>(prependBaseName("plastic_deformation_gradient"))
            ? &getADMaterialProperty<RankTwoTensor>(prependBaseName("plastic_deformation_gradient"))
            : nullptr),
    _Fe(declareADProperty<RankTwoTensor>(prependBaseName("elastic_deformation_gradient"))),
    _dpsie_dFm(declarePropertyDerivative<RankTwoTensor, true>(
        _psie_name, {prependBaseName("mechanical_deformation_gradient")})),
    _dpsie_dFp(_Fp ? &declarePropertyDerivative<RankTwoTensor, true>(
                         _psie_name, {prependBaseName("plastic_deformation_gradient")})
                   : nullptr)
{
}

void
LDElasticEnergyDensityBase::precomputeQpElasticEnergyDensity()
{
  _Fe[_qp] = _Fm[_qp];
  if (_Fp)
    _Fe[_qp] *= (*_Fp)[_qp].inverse();
}
