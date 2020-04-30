//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "PresetNodalBC.h"

class ScalarPresetBC : public PresetNodalBC
{
public:
  static InputParameters validParams();

  ScalarPresetBC(const InputParameters & parameters);

protected:
  /**
   * Evaluate the surfing BC at the current quadrature point and timestep.
   */
  virtual Real computeQpValue() override;

  const VariableValue & _scalar_val;
};
