//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "DerivativeMaterialPropertyNameInterface.h"

template <bool is_ad>
class QuadraticLocalDissipationTempl : public Material,
                                       public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  QuadraticLocalDissipationTempl(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// coupled damage variable
  const GenericVariableValue<is_ad> & _d;

  /// name of local dissipation
  const MaterialPropertyName _w_name;

  /// local dissipation
  GenericMaterialProperty<Real, is_ad> & _w;

  /// local dissipation derivative
  GenericMaterialProperty<Real, is_ad> & _dw_dd;

  /// parameter xi
  const Real & _xi;
};

// typedef QuadraticLocalDissipationTempl<false> QuadraticLocalDissipation;
typedef QuadraticLocalDissipationTempl<true> QuadraticLocalDissipation;
