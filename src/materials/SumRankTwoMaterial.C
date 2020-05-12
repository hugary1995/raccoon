//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "SumRankTwoMaterial.h"

registerADMooseObject("raccoonApp", SumRankTwoMaterial);
registerADMooseObject("raccoonApp", ADSumRankTwoMaterial);

template <bool is_ad>
InputParameters
SumRankTwoMaterialTempl<is_ad>::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("sum a list of Rank Two Tensor materials");
  params.addRequiredParam<MaterialPropertyName>(
      "sum_prop_name", "The name of the property that holds the summation");
  params.addRequiredParam<std::vector<MaterialPropertyName>>(
      "prop_names", "The names of the properties to be summed");
  return params;
}

template <bool is_ad>
SumRankTwoMaterialTempl<is_ad>::SumRankTwoMaterialTempl(const InputParameters & parameters)
  : Material(parameters),
    _sum(declareGenericProperty<RankTwoTensor, is_ad>(
        getParam<MaterialPropertyName>("sum_prop_name"))),
    _prop_names(getParam<std::vector<MaterialPropertyName>>("prop_names")),
    _num_props(_prop_names.size())
{
  _props.resize(_num_props);
  for (unsigned int i = 0; i < _num_props; i++)
    _props[i] = &getGenericMaterialProperty<RankTwoTensor, is_ad>(_prop_names[i]);
}

template <bool is_ad>
void
SumRankTwoMaterialTempl<is_ad>::initQpStatefulProperties()
{
  computeQpProperties();
}

template <bool is_ad>
void
SumRankTwoMaterialTempl<is_ad>::computeQpProperties()
{
  _sum[_qp] = 0.0;
  for (unsigned int i = 0; i < _num_props; i++)
    _sum[_qp] += (*_props[i])[_qp];
}

template class SumRankTwoMaterialTempl<false>;
template class SumRankTwoMaterialTempl<true>;
