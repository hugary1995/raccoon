//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "Function.h"

template <bool is_ad = true>
class FractureMaterialTempl : public Material
{
public:
  static InputParameters validParams();

  FractureMaterialTempl(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;

  /// constant in time
  const bool _stationary;

  /// energy release rate
  const GenericMaterialProperty<Real, is_ad> & _Gc;

  /// phase field regularization length
  const GenericMaterialProperty<Real, is_ad> & _L;

  /// norm of the local dissipation function
  const Function & _w_norm;
  // const Real & _w_norm;

  /// interface coefficient in Allen-Cahn equation
  GenericMaterialProperty<Real, is_ad> & _kappa;

  /// interface coefficient at the previous time step -- Note: 'old' properties are stored in a MaterialData class, and are non-AD by default as they should not be re-computed at every quadrature point.
  const MaterialProperty<Real> * _kappa_old;

  /// Mobility in Allen-Cahn equation
  GenericMaterialProperty<Real, is_ad> & _M;

  /// Mobility at the previous time step
  const MaterialProperty<Real> * _M_old;
};

typedef FractureMaterialTempl<false> FractureMaterial;
typedef FractureMaterialTempl<true> ADFractureMaterial;
