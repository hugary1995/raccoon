//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADWeakPlaneStress.h"
#include "Function.h"

class ADWeakOutOfPlaneStress : public ADWeakPlaneStress
{
public:
  static InputParameters validParams();

  ADWeakOutOfPlaneStress(const InputParameters & parameters);

protected:
  ADReal precomputeQpResidual() override;

  /// The out of plane stress
  const Function & _out_of_plane_stress;
};
