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

  /**
   * Compute the plastic energy and its derivatives w/r/t the (scalar) effective plastic strain
   * @param ep         The effective plastic strain
   * @param derivative The order of derivative
   * @return The plastic energy if derivative = 0, the flow stress if derivative = 1, the derivative
   * of flow stress if derivative = 2.
   */
  virtual ADReal plasticEnergy(const ADReal & ep, const unsigned int derivative = 0) = 0;

  // @{ Retained as empty methods to avoid a warning from Material.C in framework. These methods are
  // unused in all inheriting classes and should not be overwritten.
  void resetQpProperties() final {}
  void resetProperties() final {}
  // @}
};
