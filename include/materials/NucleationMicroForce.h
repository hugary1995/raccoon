//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"

class NucleationMicroForce : public Material
{
public:
  static InputParameters validParams();

  NucleationMicroForce(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;
  /// Name of the external driving force
  const MaterialPropertyName _ex_driving_name;
  /// The external driving force
  ADMaterialProperty<Real> & _ex_driving;

  /// energy release rate
  const ADMaterialProperty<Real> & _Gc;

  /// The normalization constant
  const ADMaterialProperty<Real> & _c0;

  /// phase field regularization length
  const ADMaterialProperty<Real> & _L;
  /// Lame's first parameter
  const ADMaterialProperty<Real> & _lambda;
  /// shear modulus
  const ADMaterialProperty<Real> & _mu;

  /// critical tensile strength
  const Real & _sigma_ts;

  /// critical compressive strength
  const Real & _sigma_cs;

  /// regularization length dependent parameter
  const Real & _delta;

  /// the degraded stress tensor
  const ADMaterialProperty<RankTwoTensor> & _stress;
};
