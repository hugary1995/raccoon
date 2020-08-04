//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"

template <bool is_ad>
class CriticalFractureEnergyTempl : public Material
{
public:
  static InputParameters validParams();

  CriticalFractureEnergyTempl(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// slope of the local dissipation function when d = 1
  const Function & _dw_dd;

  /// slope of the degradation function when d = 1
  const Function & _dg_dd;

  /// Mobility
  const GenericMaterialProperty<Real, is_ad> & _M;

  /// computed fracture energy barrier
  GenericMaterialProperty<Real, is_ad> & _psi_critical;
};

typedef CriticalFractureEnergyTempl<false> CriticalFractureEnergy;
typedef CriticalFractureEnergyTempl<true> ADCriticalFractureEnergy;
