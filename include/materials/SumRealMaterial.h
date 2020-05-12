//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"

template <bool is_ad>
class SumRealMaterialTempl : public Material
{
public:
  static InputParameters validParams();

  SumRealMaterialTempl(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;

  virtual void computeQpProperties() override;

  GenericMaterialProperty<Real, is_ad> & _sum;

  std::vector<MaterialPropertyName> _prop_names;

  unsigned int _num_props;

  std::vector<const GenericMaterialProperty<Real, is_ad> *> _props;
};

typedef SumRealMaterialTempl<false> SumRealMaterial;
typedef SumRealMaterialTempl<true> ADSumRealMaterial;
