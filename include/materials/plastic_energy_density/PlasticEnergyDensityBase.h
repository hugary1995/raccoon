//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "DerivativeMaterialInterface.h"
#include "ADRankTwoTensorForward.h"
#include "BaseNameInterface.h"

class PlasticEnergyDensityBase : public DerivativeMaterialInterface<Material>,
                                 public BaseNameInterface
{
public:
  static InputParameters validParams();

  PlasticEnergyDensityBase(const InputParameters & params);

  /// Set the current quadrature point
  virtual void setQp(unsigned int qp) { _qp = qp; }

  // Compute the flow stress given the effective plastic strain. This is
  // d(plastic_energy_density)/d(ep).
  virtual ADReal flowStress(const ADReal & ep) = 0;

  // Compute the derivative of the flow stress with respect to the effective plastic strain. This is
  // d2(plastic_energy_density)/d2(ep).
  virtual ADReal flowStressDerivative(const ADReal & ep) = 0;

protected:
  virtual void computeQpProperties() override;

  // This method is called before actually computing the plastic energy density. This is the chance
  // to precompute any quantities needed by the plastic energy density other than the dependent
  // state variables.
  virtual void precomputeQpPlasticEnergyDensity() {}

  /// Compute the plastic energy density
  virtual void computeQpPlasticEnergyDensity() = 0;

  // This method is called after computing the plastic energy density. This is the entry point to
  // computing thermodynamic conjugates and heat generation.
  virtual void finalizeQpPlasticEnergyDensity() {}

  /// @{ Dependent state variables
  /// The effective plastic strain
  const ADMaterialProperty<Real> & _ep;
  /// @}

  /// Name of the plastic energy density
  const MaterialPropertyName _psip_name;

  /// The plastic energy density
  ADMaterialProperty<Real> & _psip;

  /// @{ Derivatives of the plastic energy density
  ADMaterialProperty<Real> & _dpsip_dep;
  /// @}
};
