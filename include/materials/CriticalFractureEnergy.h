//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"

class CriticalFractureEnergy;

template <>
InputParameters validParams<CriticalFractureEnergy>();

class CriticalFractureEnergy : public Material
{
public:
  CriticalFractureEnergy(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// slope of the local dissipation function when d = 1
  const Function & _dw_dd;

  /// slope of the degradation function when d = 1
  const Function & _dg_dd;

  /// Mobility
  const MaterialProperty<Real> & _M;

  /// computed fracture energy barrier
  MaterialProperty<Real> & _psi_critical;
};
