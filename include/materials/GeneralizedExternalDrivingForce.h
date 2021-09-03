//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"

class GeneralizedExternalDrivingForce : public Material
{
public:
  static InputParameters validParams();

  GeneralizedExternalDrivingForce(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;
  /// Name of the external driving force
  const MaterialPropertyName _ex_driving_name;
  /// The external driving force
  ADMaterialProperty<Real> & _ex_driving;

  /// energy release rate
  const ADMaterialProperty<Real> & _Gc;

  /// phase field regularization length
  const ADMaterialProperty<Real> & _L;
  /// Lame's first parameter
  const ADMaterialProperty<Real> & _Lambda;
  /// shear modulus
  const ADMaterialProperty<Real> & _mu;

  /// critical tensile strength
  const Real & _sigma_ts;

  /// critical compressive strength
  const Real & _sigma_cs;

  /// regularization length dependent parameter
  const Real & _delta;

  /// name of the degraded stress tensor
  const MaterialPropertyName _rank_two_tensor; // remove
  /// the degraded stress tensor
  const ADMaterialProperty<RankTwoTensor> & _stress;
};
