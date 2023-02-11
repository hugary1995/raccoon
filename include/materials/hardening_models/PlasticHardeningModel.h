//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"

class PlasticHardeningModel : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();

  PlasticHardeningModel(const InputParameters & parameters);

  /// Set the current quadrature point
  virtual void setQp(unsigned int qp) { _qp = qp; }

  virtual ADReal initialGuess(const ADReal & /*effective_trial_stress*/) { return 0; }

  /**
   * Compute the plastic energy and its derivatives w/r/t the (scalar) effective plastic strain
   * @param ep         The effective plastic strain
   * @param derivative The order of derivative
   * @return The plastic energy if derivative = 0, the flow stress if derivative = 1, the derivative
   * of flow stress if derivative = 2.
   */
  virtual ADReal plasticEnergy(const ADReal & ep, const unsigned int derivative = 0) = 0;

  /**
   * Compute the plastic dissipation and its derivatives w/r/t the increment (scalar) of the
   * effective plastic strain
   * @param delta_ep     The increment of the effective plastic strain
   * @param ep           The effective plastic strain
   * @param derivative   The order of derivative
   * @return The plastic dissipation if derivative = 0, the flow stress if derivative = 1, the
   * derivative of flow stress (W.R.T. the effective plastic strain, not its increment) if
   * derivative = 2.
   */
  virtual ADReal plasticDissipation(const ADReal & /*delta_ep*/,
                                    const ADReal & /*ep*/,
                                    const unsigned int /*derivative*/)
  {
    return 0;
  }

  /**
   * Compute the thermal conjugate, i.e. $Y^\text{eq}_{,T} T$
   * @param ep           The effective plastic strain
   * @return The thermal conjugate
   */
  virtual ADReal thermalConjugate(const ADReal & /*ep*/) { return 0; }

  // @{ Retained as empty methods to avoid a warning from Material.C in framework. These methods are
  // unused in all inheriting classes and should not be overwritten.
  void resetQpProperties() final {}
  void resetProperties() final {}
  // @}
};
