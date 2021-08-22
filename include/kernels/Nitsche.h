//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ADKernel.h"
#include "BaseNameInterface.h"

class Nitsche : public ADKernel, public BaseNameInterface
{
public:
  static InputParameters validParams();

  Nitsche(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual() override;

  const ADVariableValue & _u_nodal;

  const ADVariableGradient & _grad_phi;
  const ADVariableValue & _d;
  const ADVariableGradient & _grad_d;

  const ADMaterialProperty<Real> & _wn;

  const Real _alpha;

  const ADVariableValue & _cti;
  const Real _cti_threshold;
  const Real & _cti_max;

  const bool _normal_provided;
  const RealVectorValue _normal;
};
