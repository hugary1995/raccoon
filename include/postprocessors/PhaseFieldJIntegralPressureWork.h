//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElementIntegralPostprocessor.h"
#include "BaseNameInterface.h"
#include "DerivativeMaterialPropertyNameInterface.h"

class PhaseFieldJIntegralPressureWork : public ElementIntegralPostprocessor,
                                        public BaseNameInterface,
                                        public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  PhaseFieldJIntegralPressureWork(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral() override;
  virtual Real getValue() override;

  /// Cumulative sum of the post-processor value from the old time step
  const PostprocessorValue & _integral_value_old;

  /// The gradient of phase field
  const VariableGradient & _grad_d;

  /// The applied pressure
  const ADMaterialProperty<Real> & _p;

  /// The derivative of the indicator function w.r.t. the phase field
  const ADMaterialProperty<Real> & _dI_dd;

  /// The number of displacements
  const unsigned int _ndisp;

  /// Vector of velocities
  std::vector<const VariableValue *> _u_dots;

  const RealVectorValue _t;

  const VariableGradient & _grad_q;
};
