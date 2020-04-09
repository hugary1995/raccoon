//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADMaterial.h"

// Forward Declarations
template <ComputeStage>
class Penetration;

declareADValidParams(Penetration);

template <ComputeStage compute_stage>
class Penetration : public ADMaterial<compute_stage>
{
public:
  static InputParameters validParams();

  Penetration(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  const unsigned int _ndisp;
  std::vector<const ADVariableValue *> _disp;
  const ADVariableGradient & _grad_d;
  ADMaterialProperty(Real) & _penetration;

  usingMaterialMembers;
};
