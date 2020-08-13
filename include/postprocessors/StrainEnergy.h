//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElementIntegralPostprocessor.h"

template <bool is_ad>
class StrainEnergyTempl : public ElementIntegralPostprocessor
{
public:
  static InputParameters validParams();

  StrainEnergyTempl(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral() override;
  const std::string _base_name;
  const GenericMaterialProperty<RankTwoTensor, is_ad> & _stress;
  const GenericMaterialProperty<RankTwoTensor, is_ad> & _strain;
};

typedef StrainEnergyTempl<false> StrainEnergy;
typedef StrainEnergyTempl<true> ADStrainEnergy;
