//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "DerivativeMaterialPropertyNameInterface.h"
#include "BaseNameInterface.h"

class ThinFilmInterfaceEnergyDensity : public Material,
                                       public DerivativeMaterialPropertyNameInterface,
                                       public BaseNameInterface
{
public:
  static InputParameters validParams();

  ThinFilmInterfaceEnergyDensity(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// The shear lag coefficient
  const ADMaterialProperty<Real> & _coef;

  /// Number of displacements
  const unsigned int _ndisp;

  /// Displacement variables
  std::vector<const ADVariableValue *> _disp;

  /// Name of the phase-field variable
  const VariableName _d_name;

  // @{ Interfacial energy density and its derivative w/r/t damage
  const MaterialPropertyName _psii_name;
  ADMaterialProperty<Real> & _psii;
  ADMaterialProperty<Real> & _psii_active;
  ADMaterialProperty<Real> & _dpsii_dd;
  // @}

  // @{ The degradation function and its derivative w/r/t damage
  const MaterialPropertyName _g_name;
  const ADMaterialProperty<Real> & _g;
  const ADMaterialProperty<Real> & _dg_dd;
  // @}
};
