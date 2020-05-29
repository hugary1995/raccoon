//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADMaterial.h"
#include "Function.h"

class FractureMaterial : public ADMaterial
{
public:
  static InputParameters validParams();

  FractureMaterial(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;

  /// constant in time
  const bool _stationary;

  /// energy release rate
  const MaterialProperty<Real> & _Gc;

  /// phase field regularization length
  const MaterialProperty<Real> & _L;

  /// norm of the local dissipation function
  const Function & _w_norm;

  /// interface coefficient in Allen-Cahn equation
  MaterialProperty<Real> & _kappa;

  /// interface coefficient at the previous time step
  const MaterialProperty<Real> * _kappa_old;

  /// Mobility in Allen-Cahn equation
  ADMaterialProperty<Real> & _M;

  /// Mobility at the previous time step
  const MaterialProperty<Real> * _M_old;
};
