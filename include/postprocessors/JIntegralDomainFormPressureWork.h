//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

#include "ElementIntegralPostprocessor.h"
#include "BaseNameInterface.h"
#include "DerivativeMaterialPropertyNameInterface.h"

class JIntegralDomainFormPressureWork : public ElementIntegralPostprocessor,
                                        public BaseNameInterface,
                                        public DerivativeMaterialPropertyNameInterface
{
public:
  static InputParameters validParams();

  JIntegralDomainFormPressureWork(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral() override;

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

  /// Vector of displacement gradients
  std::vector<const VariableGradient *> _grad_u;

  /// Direction of the J-integral
  const RealVectorValue _t;

  /// The domain indicator function
  const VariableValue & _q;
};
