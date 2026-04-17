//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "Material.h"
#include "BaseNameInterface.h"

class PsicDegModel : public Material, public BaseNameInterface
{
public:
  static InputParameters validParams();

  PsicDegModel(const InputParameters & parameters);

  /// Set the current quadrature point
  virtual void setQp(unsigned int qp) { _qp = qp; }

  virtual ADReal initialGuess(const ADReal & /*effective_trial_stress*/) { return 0; }

  virtual ADReal elasticEnergy(const ADReal & ep, const unsigned int derivative) = 0;

  // @{ Retained as empty methods to avoid a warning from Material.C in framework. These methods are
  // unused in all inheriting classes and should not be overwritten.
  void resetQpProperties() final {}
  void resetProperties() final {}
  // @}
};
