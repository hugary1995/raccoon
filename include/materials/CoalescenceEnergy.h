//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADMaterial.h"

class CoalescenceEnergy : public ADMaterial
{
public:
  static InputParameters validParams();

  CoalescenceEnergy(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;

  /// base name
  const std::string _base_name;

  /// coalescence mobility
  const Real & _c;

  /// Mobility in Allen-Cahn equation
  ADMaterialProperty<Real> & _M;

  /// coalescence energy
  ADMaterialProperty<Real> & _E_coalesce;

  /// degradation function
  const ADMaterialProperty<Real> & _g;

  /// degradation function
  const ADMaterialProperty<Real> & _w;

  /// effective plastic strain
  const ADMaterialProperty<Real> & _ep;
};
