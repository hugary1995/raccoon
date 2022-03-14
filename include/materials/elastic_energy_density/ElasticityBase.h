//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "DerivativeMaterialInterface.h"
#include "ADRankTwoTensorForward.h"
#include "BaseNameInterface.h"

class ElasticityBase : public DerivativeMaterialInterface<Material>, public BaseNameInterface
{
public:
  static InputParameters validParams();

  ElasticityBase(const InputParameters & parameters);

  /// Set the current quadrature point
  virtual void setQp(unsigned int qp) { _qp = qp; }

protected:
  virtual void computeQpProperties() override;

  // This method is called before actually computing the elastic energy density. This is the chance
  // to precompute any quantities needed by the elastic energy density other than the dependent
  // state variables.
  virtual void precomputeQpElasticEnergyDensity() {}

  /// Compute the elastic energy density
  virtual void computeQpElasticEnergyDensity() = 0;

  // This method is called after computing the elastic energy density. This is the entry point to
  // computing thermodynamic conjugates and heat generation.
  virtual void finalizeQpElasticEnergyDensity() {}

  /// Name of the elastic energy density
  const MaterialPropertyName _psie_name;

  /// The elastic energy density
  ADMaterialProperty<Real> & _psie;

  /// The elastic strain
  const ADMaterialProperty<RankTwoTensor> & _mechanical_strain;

  /// @{ Derivatives of the elastic energy density
  ADMaterialProperty<RankTwoTensor> & _dpsie_dmechanical_strain;
  /// @}
};
