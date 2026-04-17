//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "Material.h"

/**
 * Compute density depending on strains from deforming mesh.
 */
template <bool is_ad>
class StrainAdjustedDensityCustomTempl : public Material
{
public:
  static InputParameters validParams();

  StrainAdjustedDensityCustomTempl(const InputParameters & params);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;

  const std::string _base_name;
  const GenericMaterialProperty<Real, is_ad> & _strain_free_density;

private:
  GenericMaterialProperty<Real, is_ad> & _density;
  const ADMaterialProperty<RankTwoTensor> & _F;
};

typedef StrainAdjustedDensityCustomTempl<false> StrainAdjustedDensityCustom;
typedef StrainAdjustedDensityCustomTempl<true> ADStrainAdjustedDensityCustom;
