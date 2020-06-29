//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADMaterial.h"
#include "MaterialPropertyUserObject.h"

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

  /// coalescence coefficients
  const Real & _beta;
  const Real & _e0;

  /// Mobility in Allen-Cahn equation
  ADMaterialProperty<Real> & _M;
  const ADMaterialProperty<Real> & _M0;

  /// degradation function
  const ADMaterialProperty<Real> & _w;

  /// effective plastic strain
  const ADMaterialProperty<Real> & _ep;
};
