//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADMaterial.h"
#include "Function.h"

template <ComputeStage>
class CrackSurfaceDensity;

declareADValidParams(CrackSurfaceDensity);

template <ComputeStage compute_stage>
class CrackSurfaceDensity : public ADMaterial<compute_stage>
{
public:
  static InputParameters validParams();

  CrackSurfaceDensity(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  /// norm of the local dissipation function
  const Function & _c0;

  /// phase field regularization length
  const MaterialProperty<Real> & _L;

  /// gradient of damage
  const ADVariableGradient & _grad_d;

  /// local dissipation function
  const ADMaterialProperty(Real) & _w;

  /// crack surface density
  ADMaterialProperty(Real) & _gamma;

  /// crack surface normal (if well-defined)
  ADMaterialProperty(RealVectorValue) & _n;

  usingMaterialMembers;
};
