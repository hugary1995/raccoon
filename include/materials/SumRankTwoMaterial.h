//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "RankTwoTensor.h"

template <bool is_ad>
class SumRankTwoMaterialTempl : public Material
{
public:
  static InputParameters validParams();

  SumRankTwoMaterialTempl(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;

  virtual void computeQpProperties() override;

  GenericMaterialProperty<RankTwoTensor, is_ad> & _sum;

  std::vector<MaterialPropertyName> _prop_names;

  unsigned int _num_props;

  std::vector<const GenericMaterialProperty<RankTwoTensor, is_ad> *> _props;
};

typedef SumRankTwoMaterialTempl<false> SumRankTwoMaterial;
typedef SumRankTwoMaterialTempl<true> ADSumRankTwoMaterial;
