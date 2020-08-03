//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#include "StrainEnergy.h"
#include "metaphysicl/raw_type.h"

registerMooseObject("raccoonApp", StrainEnergy);
registerMooseObject("raccoonApp", ADStrainEnergy);

template <bool is_ad>
InputParameters
StrainEnergyTempl<is_ad>::validParams()
{
  InputParameters params = ElementIntegralPostprocessor::validParams();
  params.addClassDescription("computes the total strain energy of the form $\\int_\\body 0.5 "
                             "\\sigma \\colon \\varepsilon \\diff{V}$.");
  params.addParam<std::string>("base_name", "base name for stress and strain");
  return params;
}

template <bool is_ad>
StrainEnergyTempl<is_ad>::StrainEnergyTempl(const InputParameters & parameters)
  : ElementIntegralPostprocessor(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _stress(getGenericMaterialProperty<RankTwoTensor, is_ad>(_base_name + "stress")),
    _strain(getGenericMaterialProperty<RankTwoTensor, is_ad>(_base_name + "mechanical_strain"))
{
}

template <bool is_ad>
Real
StrainEnergyTempl<is_ad>::computeQpIntegral()
{
  return 0.5 * MetaPhysicL::raw_value(_stress[_qp].doubleContraction(_strain[_qp]));
}

template class StrainEnergyTempl<false>;
template class StrainEnergyTempl<true>;
