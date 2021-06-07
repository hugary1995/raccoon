//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "StressDivergenceTensors.h"

class LowerDimensionalStressDivergenceTensors : public StressDivergenceTensors
{
public:
  static InputParameters validParams();

  LowerDimensionalStressDivergenceTensors(const InputParameters & parameters);

protected:
  Real computeQpResidual() override;
  Real computeQpJacobian() override;
  Real computeQpOffDiagJacobian(unsigned int jvar) override;

  /// Thickness of the lower dimensional block
  const MaterialProperty<Real> & _thickness;
};
